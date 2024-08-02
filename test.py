#!/usr/bin/python3

## import the required libraries
import os
import urllib.parse

## print a HTTP content header
print('Content-type: text/plain\r\n')

## get the query string. this gets passed to cgi scripts as the environment
## variable QUERY_STRING
query_string = os.environ['QUERY_STRING']

## convert the query string to a dictionary
arguments = urllib.parse.parse_qs(query_string)

## print out the values of each argument
for name in arguments.keys():
    ## the value is always a list, watch out for that
    print(str(name) + ' = ' + str(arguments[name]))


# import cgi
# import cgitb; cgitb.enable() # Optional; for debugging only
# import sys

# print("Content-Type: text/html")
# print('')

# # arguments = cgi.parse()
# # for i in arguments.keys():
# #  print(arguments[i].value)

# print(sys.stdin.read())
