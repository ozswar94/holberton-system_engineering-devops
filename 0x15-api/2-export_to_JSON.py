#!/usr/bin/python3
""" export json on json file """
import json
import requests
import sys


if __name__ == '__main__':
    id = str(sys.argv[1])
    u_url = "https://jsonplaceholder.typicode.com/users/{}".format(id)
    t_url = "https://jsonplaceholder.typicode.com/users/{}/todos".format(id)

    user = requests.get(u_url).json()
    todo = requests.get(t_url).json()

    content_json = {id: []}
    for t in todo:
        content = {
                    "task": t.get("title"),
                    "completed": t.get("completed"),
                    "username": user.get('username')
                }
        content_json['2'].append(content)

    filename = "{}.json".format(id)
    with open(filename, 'w') as f:
        json.dump(content_json, f)
