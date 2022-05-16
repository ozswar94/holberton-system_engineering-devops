#!/usr/bin/python3
""" REST Api """
import requests
import sys


if __name__ == '__main__':
    id = str(sys.argv[1])
    u_url = "https://jsonplaceholder.typicode.com/users/{}".format(id)
    t_url = "https://jsonplaceholder.typicode.com/users/{}/todos".format(id)

    user = requests.get(u_url).json()
    todo = requests.get(t_url).json()

    number_of_task = 0
    total_of_task = 0
    complete_task = []

    for t in todo:
        total_of_task += 1
        if t.get("completed") is True:
            number_of_task += 1
            complete_task.append(t.get('title'))

    p = "Employee {} is done with tasks({}/{}):"
    print(p.format(user.get('name'), number_of_task, total_of_task))
    for t in complete_task:
        print('\t {}'.format(t))
