#!/usr/bin/python
##################################################################################
#
#    ktouchgen.py         Builds Levelfiles for ktouch
#    This is an enhanced reimplementation of a programm written by
#        Haavard Froeiland <havard@student.unsw.edu.au>
#   
#    This Version was written by
#        Hendrik Naumann <hn75@gmx.de>
#        License: GPL
#        Last edited:  11.10.2001
##################################################################################
#
#    SYNTAX OF THE CONFIGFILE
#      
#      Sections:
#          [Main]
#          level_rows =       Generated rows per level
#          row_length =       Length of the generated rows
#
#          [Level<Num>]       Settings for the Levels to create
#          lchars =           Chars that must be in the words
#                             in the following levels these chars will be
#                             permitted to be in the words
#          title =            Title of the Level. If not given it is set to
#                             lchars
#          rows =             Number of rows. This overwrites level_rows.
#          type = 0           Wordlist will be used for Level. If type is
#                             not given this is default.
#          type > 0           Words will be created from lchars and permitted
#                             chars. The number of type indicates the length
#                             of the genrated words.
#
##################################################################################

from whrandom import randint, random
import string
from string import join, maketrans, translate, count, strip, lower, find, upper
import time
import sys
import ConfigParser
import regex

DOCSTRING = """
Usage:
   ./ktouchgen.py  wordfile configfile outputfile
       wordfile     Is an file containing the words the levels are build from.
                    It should contain one word on each line
       configfile   File that contains the configuration of the levels.
                    See ktouchgen.py for documentation.
       outputfile   The name of the new levelfile. If it exists it will
                    be overwritten.
"""

class LevelList:
    "Level List Class"
    def __init__(self, Levelchars, Permitchars):
        self.list = {0:[join(Levelchars,"")]}
        self.wordcount = 0.0
        self.llist = Levelchars
        self.plist = Permitchars

    def SelectWords(self, Wordlist):
        """
        SelectWords(self, Wordlist)
        Searches for words only contain Permitchars and at least
        one Levelchar.
        Calculate the number of levelchars / per chars of the word
        and fill this values in an mapping
        {lchars/chars*1000 :[list of words with this property]}
        """
        Transstring = maketrans("","")
        pliststring = join(self.plist, "")
        lliststring = join(self.llist, "")

        for Word in Wordlist:
            lchar_count = 0
            if len(translate(Word, Transstring, pliststring)) == 0:
                lchar_count = len(Word) - len(translate(Word, Transstring, lliststring))
                if lchar_count:
                    weight = int((float(lchar_count) / float(len(Word))) * 1000)
                    if self.list.has_key(weight):
                        self.list[weight].append(Word)
                    else:
                        self.list[weight] = [Word]
                        self.wordcount = self.wordcount + 1
        
    def GetRandomList(self, listlength):
        """
        GetRandomList(self, listlength)
        Returns a list of randomwords with listlength length.
        First choose words with most Levelchars, if these are
        not enough, words with less are chosen.
        """
        retlist = []
        selectlist = []
        length = 0
        val = 0
        temp = 0
        keys = self.list.keys()
        keys.sort()
        keys.reverse()
        for key in keys:
            if length < listlength:
                for count in range(len(self.list[key]) - 1): 
                    if length < listlength and key > 0 :
                        num = randint (0, len(self.list[key]) - 1)
                        word = self.list[key][num]
                        temp = temp + key
                        del(self.list[key][num])
                        val = val + 1
                        length = length + len(word)
                        selectlist.append(word)
                    else:
                        break
            else:
                break
        temp = float(temp) / val / 10
        print 'Got words with an averages of %(temp).2f %% lchars.' %vars()
        # Select the returnlist from selectlist
        val = val - 1
        length = 0
        while length < listlength:
            word = selectlist[randint(0, val)]
            length = length + len(word)
            retlist.append(word)

        return retlist

    def GenArtWord(self, Wordlength):
        """
        GenArtWord(self, Wordlength)
        Builds an artifical word (with length Wordlength) out of Levelchars and Permitchars.
        Does it like: random(lchar) + random(pchar) + .....
        """
        ret = ""
        while len(ret) < Wordlength:
            ret = ret + self.llist[randint(0, len(self.llist) - 1)] + self.plist[randint(0, len(self.plist) - 1)]
        return ret

    def GetArtList(self, Listlength, Wordlength):
        """
        GetArtList(self, Listlength, Wordlength)
        Buids an Wordlist with length Listlength out of artificial words.
        See: self.GenArtWord()
        """
        length = 0
        ret = []
        while length < Listlength:
            word = self.GenArtWord(Wordlength)
            ret.append(word)
            length = length + len(word)
        return ret
    

def main(argv):
    Wordlist = []
    UpcaseWordlist = []
    # Reading the Wordlist
    try:
        wordfile = open(argv[1], 'r')
    except IOError:
        print "\nWordfile couldn't be opened.\n", DOCSTRING
        return 1
    # Create two Wordlists, one with first char lowered
    # (more words for the first levels) and one like it ist read
    for wordstring in wordfile.readlines():
        wordstring = strip(wordstring)
        if lower(wordstring) != wordstring:
            UpcaseWordlist.append(wordstring)
        Wordlist.append(lower(wordstring))
    wordfile.close()

    # Parse the configfile
    # Creates a List Levelops with [(Options), ]
    # Optiontuple contains (lchars, title, rows)
    conf = ConfigParser.ConfigParser()
    try:
        file = open(argv[2],'r')
    except IOError:
        print '\nConfigfile could not be opened.\n', DOCSTRING
        return 1
    file.close()
    conf.read(argv[2])
    try:
        Rowlength = conf.getint('Main', 'row_length')
    except ConfigParser.MissingSectionHeaderError:
        print '\nWrong configfile. See ktouchgen.py for Documentation.' + DOCSTRING
    Levelrows = conf.getint('Main', 'level_rows')
    Levelops = []
    Levelnum = 1
    section = 'Level' + str(Levelnum)
    while conf.has_section(section):
        lchars = []
        try:
            for char in strip(conf.get(section, 'lchars')):
                lchars.append(char)
        except ConfigParser.NoOptionError:
            print '\nNo characters defined for level %(Levelnum)s !' %vars()
            return 1
        try:
            title = conf.get(section, 'title')
        except ConfigParser.NoOptionError:
            title = join(lchars)
        try:
            rows = conf.getint(section, 'rows')
        except ConfigParser.NoOptionError:
            rows = Levelrows
        try:
            type = conf.getint(section, 'type')
        except ConfigParser.NoOptionError:
            type = 0

        Levelops.append((lchars, title, rows, type))
        Levelnum = Levelnum + 1
        section = 'Level' + str(Levelnum)
    print '\nConfiguration for %(Levelnum)s levels read. \n!!! Be aware, if the Levels are not numberd correctly \n!!! they will not be read completely!' %vars()

    # Generate Output
    try:
        outfile = open(argv[3], 'w')
    except IOError:
        print "Outputfile could not be opened.\n", DOCSTRING
        return 1
    outfile.write('#########################################################\n' +\
                  '# Trainingfile generaded ' + time.ctime(time.time()) + '\n' +\
                  '# Program written by Hendrik Naumann <hn75@gmx.de>\n' +\
                  '# Inspired by Håvard Frøiland\'s version\n' +\
                  '#########################################################\n')
    permit_chars = []
    Levelnum = 0
    for Option in Levelops:
        cachestring = ""
        Levelnum = Levelnum + 1
        for new_char in Option[0]:
            if  new_char not in join(permit_chars,""):
                permit_chars.extend(Option[0])
        outfile.write('\n# Level %(Levelnum)s\n' %vars() + Option[1] + "\n")

        print "Generating Level " + str(Levelnum)
        print join(permit_chars,"")

        # Generate a LevelList object and give the needed Wordlists
        levelwordlist = LevelList (Option[0], permit_chars)
        if Option[3] == 0:
            if lower(join(permit_chars,"")) != join(permit_chars,""):
                if upper(join(Option[0],"")) != join(Option[0],""):
                    levelwordlist.SelectWords(Wordlist)
                levelwordlist.SelectWords(UpcaseWordlist)
            else:
                levelwordlist.SelectWords(Wordlist)
            randomlist = levelwordlist.GetRandomList(Rowlength * Option[2])
        else:
            randomlist = levelwordlist.GetArtList(Rowlength * Option[2], Option[3])

        # Write the randomlist
        for word in randomlist:
            cachestring = cachestring + " " + word
            if len(cachestring) > Rowlength - 3:
                outfile.write(strip(cachestring) + "\n")
                cachestring = ""
    outfile.close()
    return 0

if __name__ == "__main__":
    if len(sys.argv) == 4:
        main(sys.argv)
    else:
        print '\nWrong number of parameters\n' + DOCSTRING
