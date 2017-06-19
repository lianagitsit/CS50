from flask import Flask, redirect, render_template, request, url_for

import helpers
import os
import sys
from analyzer import Analyzer
from nltk.tokenize import TweetTokenizer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name)

    # TODO
    if tweets == None:
        return redirect(url_for("index"))
    
    tknzr = TweetTokenizer()
    
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)
    
    positive, negative, neutral = 0.0, 0.0, 0.0
    
    for tweet in tweets:
        print(tknzr.tokenize(tweet))
        # analyze tweet
        score = analyzer.analyze(tweet)
        if score > 0.0:
            # positive = (positive / len(tweets)) * 100
            positive = positive + 1
            print("positive: {}".format(positive))
        elif score < 0.0:
            negative = negative + 1
            print("negative: {}".format(negative))
        else:
            neutral = neutral + 1
            
    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
