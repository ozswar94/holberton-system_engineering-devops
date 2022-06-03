#!/usr/bin/python3
""" export to csv """
import csv
import requests
import sys


if __name__ == '__main__':
    id = sys.argv[1]
    u_url = "https://jsonplaceholder.typicode.com/users/{}".format(id)
    t_url = "https://jsonplaceholder.typicode.com/users/{}/todos".format(id)

    user = requests.get(u_url).json()
    todo = requests.get(t_url).json()

    with open("{}.csv".format(id), 'w') as f:
        writer = csv.writer(f, quoting=csv.QUOTE_ALL)
        for t in todo:
            line = [
                    id,
                    user.get('username'),
                    t.get('completed'),
                    t.get('title')
                    ]
            writer.writerow(line)
