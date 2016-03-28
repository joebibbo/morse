import sys
import re

#
#
def patternSubDelegate(matchObj):
    santize = matchObj.re.pattern.replace('\\','');

    result = '';
    n = 0;
    for ch in santize:
        if ch == '.':
            result = result + matchObj.string[n];

        n = n + 1;

    return result

def getPatterns( message, hidden):
    masks = [];

    template = ""
    for  n in range(len(message)):
        template = template + '.';

    masks.extend([template]);    
    masks.extend(['...' + hidden]);
    masks.extend([message]);

    return masks;

# add new unique pattern. Check to see if pattern already exists.
def addToDictionary(patterns, pattern):
    if ( not (pattern in patterns.keys()) ):
       patterns[pattern] = 0

    return

def findUnique( message, hidden, patterns):

    if ( len( message ) == len ( hidden ) ):
        if message == hidden:
            addToDictionary(patterns, hidden);
    else:
        # need to recursively call with a smaller message
        findUnique(message[1:], hidden);

        template = ""
        for  n in range(len(message)):
            template = template + '.';

        # we can save ourselves some time if the first character of the hidden doesn't match then move on
#        if ( hidden[0] == message[0] ):

    return;

def walkMasks(indices, pos, message, hidden, patterns):

    print pos
    print indices;
    if pos == len(hidden) - 1:
        curPos = indices[pos];
        pattern = indices;
        for index in range(curPos, len(message) - 1):
            pattern[pos] = index
            patterns.extend(pattern)
    else:
        if ( indices[pos] < ( len(message) - len(hidden ) )):
            indices[pos] = indices[pos] + 1;
            pos = pos + 1
            walkMasks(indices, pos, message, hidden, patterns)
         
            print indices;

    return;

def main():

    if len(sys.argv) != 4:
        print 'Usage: hidden.py message hidden1 hidden2  # strings are expected to be encoded as morse code, as per the assignment.'
        sys.exit(0);

    if len(sys.argv[1]) > 100:
        print 'Input message exceeds the bounds of the assignment: length = ' + str( len(sys.argv[1]) ) + ' tokens'
        sys.exit(0);

    messages = [];

    message = sys.argv[1]
    hidden1 = sys.argv[2]
    hidden2 = sys.argv[3]

    #message = '*- -***'
    remove  = hidden1

    # Remove all letter/word delimiters (ie spaces) , as they problem is not concerned with displaying the unique patterns just counting them.
    message = message.replace(' ','')
    remove  = remove.replace(' ', '')

    pattern = r'\*\-.\*..'
    match = re.search(pattern, message);

    if match:
        hidden = re.sub(pattern, patternSubDelegate, message);
        print 'found: masked pattern - ', hidden
    else:
        print 'did not find'

    #patterns = getPatterns(message, hidden);

    patterns = [];
    #findUnique(message, hidden, patterns)

    #for next in patterns:
    #    print next;

    pos     = 0;
    indices = range(len(hidden1));
    walkMasks(indices, pos, message, hidden1, patterns);

if __name__ == '__main__':
    main()