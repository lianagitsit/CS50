from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    # list of dicts within portfolio table
    # select all rows in portfolio that share the user id currently logged in
    stock_list = db.execute("SELECT * FROM portfolio WHERE user_id = :user_id", user_id=session.get("user_id"))
    c = db.execute("SELECT cash FROM users WHERE id = :id", id=session.get("user_id"))
    cash = c[0]["cash"]
    stock_total = 0

    # patch to fix database after adding new column
    # namePatch = db.execute("UPDATE transactions SET name = :name WHERE symbol = 'aapl'", name=lookup("aapl")["name"])
    
    # for each dict (row) in the list of transactions
    for stock_dict in stock_list:
        current_price = lookup(stock_dict["symbol"])["price"]
        # adds new key-value pair for current price
        stock_dict["current_price"] = current_price
        stock_total = stock_total + (current_price * stock_dict["current_shares"])
        

    return render_template("index.html", stock_list=stock_list, cash=cash, stock_total=stock_total, usd=usd)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    if request.method == "POST":
        if not request.form.get("buy"):
            return apology("stock symbol required")
        
        elif not request.form.get("shares"):
            return apology("number of shares required")
        
        elif int(request.form.get("shares")) < 0:
            return apology("must enter positive share number")
        
        else:
            symbol = request.form.get("buy").upper()
            quote = lookup(symbol)
            if quote == None:
                return apology("invalid stock symbol")

            purchase = float(quote["price"]) * float(request.form.get("shares"))
            cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session.get("user_id"))

            # check that user has enough cash to make the purchase
            if cash[0]["cash"] > purchase:
                
                # if the user already owns shares of the stock:
                if len(db.execute("SELECT * FROM portfolio WHERE user_id = :user_id AND symbol = :symbol", user_id=session.get("user_id"), symbol=symbol)) == 1:
                    
                    # update the current number of shares that they own within the portfolio
                    db.execute("UPDATE portfolio SET current_shares = current_shares + :shares WHERE user_id = :user_id AND symbol = :symbol", \
                        shares=request.form.get("shares"), user_id=session.get("user_id"), symbol=symbol)
                
                # if the user does not already own shares of the stock:
                if len(db.execute("SELECT * FROM portfolio WHERE user_id = :user_id AND symbol = :symbol", user_id=session.get("user_id"), symbol=symbol)) == 0:
                    # add a row to the portfolio table with stock and share info
                    db.execute("INSERT INTO 'portfolio' (user_id, symbol, current_shares, name) VALUES(:user_id, :symbol, :current_shares, :name)", \
                        user_id=session.get("user_id"), symbol=symbol, current_shares=(request.form.get("shares")), name=quote["name"])
                
                # update master transaction table with purchase info
                db.execute("INSERT INTO 'transactions' (user_id, symbol, price, shares, name) VALUES(:user_id, :symbol, :price, :shares, :name)", \
                    user_id=session.get("user_id"), symbol=symbol, price=quote["price"], shares=request.form.get("shares"), name=quote["name"])
                
                # update the user's cash amount to reflect the purchase
                db.execute("UPDATE users SET cash = cash - :purchase WHERE id = :id", purchase=purchase, id=session.get("user_id"))
            
            # if the user does not have enough cash to purchase
            elif cash[0]["cash"] < purchase:
                # render apology
                return apology("not enough cash")
            
            return redirect(url_for("index"))
    
    if request.method == "GET":
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    # list of dicts within transactions table
    # select all rows in transactions that share the user id currently logged in
    transactions_list = db.execute("SELECT * FROM transactions WHERE user_id = :user_id", user_id=session.get("user_id"))
    
    return render_template("history.html", transactions_list=transactions_list, usd=usd)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    print("Quote got called")
    if request.method == "POST":
        
        quote = lookup(request.form.get("quote"))
        
        if quote == None:
            return apology("invalid stock symbol")
        
        else:
            return render_template("quoted.html", value1=quote["name"], value2=quote["price"], value3=quote["symbol"])
        
    if request.method == "GET":
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    session.clear()
    
    if request.method == "POST":
        
        if not request.form.get("username"):
            return apology("must provide username")
        
        elif not request.form.get("password"):
            return apology("must provide password")
        
        elif not request.form.get("password2"):
            return apology("must confirm password")
        
        elif request.form.get("password") != request.form.get("password2"):
            return apology("password entries must match")
        
        else:
            # query database for username
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            
            # if username is already in database
            if len(rows) == 1:
                return apology("username taken")
                
            # if username is unique (NOT already in database)
            if len(rows) != 1:
                # hash password and insert username and hash into database
                hash = pwd_context.encrypt(request.form.get("password"))
                db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=hash)
                
                #select registered username from database
                result = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
                
                # remember which user has logged in
                session["user_id"] = result[0]["id"]

                # redirect user to home page
                return redirect(url_for("index"))

    else:
        return render_template("register.html")
    
    # return apology("TODO")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        
        if not request.form.get("sell_symbol"):
            return apology("stock symbol required")
            
        elif not request.form.get("sell_shares"):
            return apology("number of shares required")
        
        elif int(request.form.get("sell_shares")) < 0:
            return apology("positive number of shares required")
        
        else:
            
            user_symbol = request.form.get("sell_symbol").upper()
            quote = lookup(user_symbol)
            if quote == None:
                return apology("invalid stock symbol")

            sale = float(lookup(user_symbol)["price"]) * float(request.form.get("sell_shares"))
            cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session.get("user_id"))
            
            # query databse for user's portfolio
            portfolio_list = db.execute("SELECT * FROM portfolio WHERE user_id = :user_id AND symbol = :symbol", user_id=session.get("user_id"), symbol=user_symbol)
            
            # check if user owns stock
            # for stock in range(len(portfolio_list)):

            if len(portfolio_list) == 0:
                return apology("stock not in portfolio")

                
            # if the user already owns shares of the stock:
            if len(portfolio_list) == 1:
                
                #if portfolio_list[stock]["symbol"] == user_symbol:

                # check if user has enough stock to sell desired amount
                if int(request.form.get("sell_shares")) < portfolio_list[0]["current_shares"]:
                        
                    # subtract sold shares from current shares
                    db.execute("UPDATE portfolio SET current_shares = current_shares - :sell_shares WHERE user_id = :user_id AND symbol = :sell_symbol", \
                        sell_shares=int(request.form.get("sell_shares")), user_id=session.get("user_id"), sell_symbol=user_symbol)
                        
                    # update master transaction table with purchase info
                    db.execute("INSERT INTO 'transactions' (user_id, symbol, price, shares, name) VALUES(:user_id, :symbol, :price, :shares, :name)", \
                        user_id=session.get("user_id"), symbol=user_symbol, price=quote["price"], shares=(0 - int(request.form.get("sell_shares"))), name=quote["name"])
                
                    # update the user's cash amount to reflect the purchase
                    db.execute("UPDATE users SET cash = cash + :sale WHERE id = :id", sale=sale, id=session.get("user_id"))
                    
                # if user sells all shares of stock
                elif int(request.form.get("sell_shares")) == portfolio_list[0]["current_shares"]:
                        
                    # update master transaction table with purchase info
                    db.execute("INSERT INTO 'transactions' (user_id, symbol, price, shares, name) VALUES(:user_id, :symbol, :price, :shares, :name)", \
                        user_id=session.get("user_id"), symbol=user_symbol, price=quote["price"], shares=(0 - int(request.form.get("sell_shares"))), name=quote["name"])
                
                    # update the user's cash amount to reflect the purchase
                    db.execute("UPDATE users SET cash = cash + :sale WHERE id = :id", sale=sale, id=session.get("user_id"))
                        
                    # delete stock from portfolio
                    db.execute("DELETE FROM portfolio WHERE user_id = :user_id AND symbol = :sell_symbol", \
                        user_id=session.get("user_id"), sell_symbol=user_symbol)
                    
                # if user does not have enough shares to sell, apology
                elif int(request.form.get("sell_shares")) > portfolio_list[0]["current_shares"]:
                    return apology("not enough shares to sell")

            return redirect(url_for("index"))
            
    elif request.method == "GET":
        return render_template("sell.html")