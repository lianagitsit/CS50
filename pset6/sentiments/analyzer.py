import nltk
from nltk.tokenize import TreebankWordTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.poslines = []
        with open(positives) as pfile:
            for line in pfile:
                if line.startswith(';') == False and line.startswith('\n') == False:
                    line = line.strip()
                    self.poslines.append(line)

        self.neglines = []
        with open(negatives) as nfile:
            for line in nfile:
                if line.startswith(';') == False and line.startswith('\n') == False:
                    line = line.strip()
                    self.neglines.append(line)

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        tokenizer = TreebankWordTokenizer()
        tokens = tokenizer.tokenize(text)
        #tokens = t.lower()
        score = 0
        pscore = 0
        nscore = 0
        
        for i in self.poslines:
            for j in tokens:
                if i == j.lower():
                    pscore = pscore + 1
        # print("positive word score: {}".format(pscore))
        
        for i in self.neglines:
            for j in tokens:
                if i == j.lower():
                    nscore = nscore - 1
        # print("negative word score: {}".format(nscore))
        
        score = pscore + nscore
        # print("total score: {}".format(score))
        
        return score