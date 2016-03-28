// morse.cpp : 
//
// This is a self-contained console application to solve a specific interview problem. The problem is decribed in 
// detail in the an accompaining document 'Hidden Message - Handout.docx'
//
// The programming challenge is to write an efficient solution of finding all the deletion paths of a given message.
// A deletion path is defined as the remaining characters of a string after all the paths/combinations of another
// are removed ( we will call this string the hidden message ).
//
// So given a 'message' with N characters, find all the deletion paths after removing the characters of a 'hidden' 
// message with M characters. The orders of the characters in 'hidden' message must be maintained.
//
// This particular problem with uses tokens to represent the morse code equivalent of a textual string. See the detailed
// documentation for more information.
//
// The test case of message = 'HELLO WORLD' hidden = 'HELP' must execute in less than 10 secs and return 1311. The input
// strings are supplied through the command line, and are encoded into their morse code representation.
//
//
// The Solution:
// The solution become an exercise in walking a spanning tree of all the possible combinations of M tokens (hidden) in the N token string(message).
// The tree should be reduce to prune any branches that do require traversal due to higher order misses.
//
// Programmer: Joe A. Bibbo
// Date: 3/28/2016
//

#include "stdio.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
#include <unordered_map>
#include <time.h>

using namespace std;

inline bool getDeletionPath(string& message, string& hidden, string& deletionPath, vector<int>& path);

class SpanningTree
{
    typedef struct TreeDimens
    {
        TreeDimens() {};

        void reset()
        {
            deletionPaths.clear();
        }

        void set(string& message_, string& hidden_)
        {
            message = message_;
            hidden  = hidden_;

            N = (int) message.size();
            M = (int) hidden.size();
        };

        int N;
        int M;
        string hidden;
        string message;
        
        //use std::
        void addDeletionPath(string& path)         
        { 
            deletionPaths[path] = true; 
        };

        //map< string, bool> deletionPaths;
        unordered_map< string, bool> deletionPaths;

    } TreeDimen;

    typedef struct TreeNode
    {

        void Traverse(int level, int pos, TreeDimen& dimen,  vector<int>& path) 
        {
            if ( dimen.message[pos] != dimen.hidden[level] )
            {
               return;
            }

            _pos   = pos;
            _level = level;
            path[level] = pos;

            if (level == dimen.M - 2)
            {
                // Next level are the leaves
                for (int pos = _pos + 1; pos < dimen.N; pos++)
                {
                    path[level+1] = pos;

                    if ( dimen.message[pos] == dimen.hidden[level+1] )
                    {
                        string deletionPath;
                        if (getDeletionPath(dimen.message, dimen.hidden, deletionPath, path))
                            dimen.addDeletionPath( deletionPath );
                    }
                }
            }
            else
            {
                int left    = dimen.M - _level;
                int lastPos = dimen.N - left + 1;
                for ( int pos= _pos+1; pos<= lastPos; pos++)
                {
                    TreeNode node;
                    node.Traverse(_level+1, pos, dimen, path);
                }
            }
        }

        private: 
            int  _pos, _level;
            
    } TreeNode;

    public:
        SpanningTree() { };

        void Walk(string& message, string& hidden, string& hidden2)
        {
            _dimen.reset();

            if ( walk(message, hidden) )
            {
                if ( hidden2.empty() )
                    // ================== Problem 1 of the assignment
                    printf("# of deletion paths = %d, for hidden message: %s\n", _dimen.deletionPaths.size(), hidden.c_str() );
                else
                {
                    printf("# of deletion paths = %d after first hidden message: %s\n", _dimen.deletionPaths.size(), hidden.c_str() );
                    // ================== Problem 2 of the assignment
                    auto deletionPathsFirstPass = _dimen.deletionPaths;
                    _dimen.reset();
  
                    int n=0;
                    for (auto path : deletionPathsFirstPass)
                    {
                        string newMessage = path.first;
                        //printf("%d: %s\n ", n++, newMessage.c_str());
                        walk( newMessage, hidden2 );
                    }

                    printf("# of deletion paths = %d, for hidden messages: %s and %s\n", _dimen.deletionPaths.size(), hidden.c_str(), hidden2.c_str() );
                }
            }
        }

    private:
        bool walk(string& message, string& hidden)
        {
            bool success = false;

            if ( message.size() >= hidden.size() )
            {
                _dimen.set( message, hidden );

                vector<int> path( _dimen.M );

                for (int n = 0; n <= (_dimen.N - _dimen.M); n++)
                {
                    TreeNode node;
                    node.Traverse( 0, n, _dimen, path );
                }

                success = true; 
            } 
            
            return success;
        }

        TreeDimens        _dimen;
};


/*
    Test case for debugging purpose only ...

    problem 1
    command line arguments: "**** * *-** *-** ---   *-- --- *-* *-** -**" "**** * *-** *--*"

    problem 2
    command line arguments: "-_**** *   *** - *- *-*   *-- *- *-* ***   *** *- --* *-" "-*-- --- -** *-" "*-** * ** *-"
*/

int main(int argc, char* argv[])
{
    if ((argc < 3) || ( argc >4))
    { 
        printf("Usage: > morse \"message\" \"hidden message 1\" <\"hidden message 2\">\n " );
        exit(0);
    }

    string hidden, hidden2;
    string message;

    message = argv[1];
    hidden  = argv[2];
    if ( argc == 4 )
        hidden2 = argv[3];

    time_t start, end;
    time(&start);
    
    SpanningTree tree;
    tree.Walk( message, hidden, hidden2 );

    time(&end);
    double seconds = difftime( end, start );
    printf("execution time: %.f secs (rounded) \n", seconds);

	return 0;
}

// Using a message, a hidden message (mask) and path from our spanning tree see if we have a 'hit'. Return the deletion path in 'path' and return
// true to indicate valid deletion path, otherwise we are returning false.
inline bool getDeletionPath(string& message, string& hidden, string& deletionPath, vector<int>& path)
{
    bool found = false;
    
    if (message.size() && hidden.size() && path.size() && ( path.size() == hidden.size() ) )
    {
        string temp = "";
        deletionPath.clear();

        int m = 0;
        for (int n = 0; n < (int) message.size(); n++)
        {
            if (!found && (n == path[m]))
            {
                if (hidden[m] == message[n])
                {
                    m++;
                    if (m == path.size() )
                        found = true;
                }
                else
                    break;
            }
            else
                deletionPath += message[n];
        }
    }

    return found;
}