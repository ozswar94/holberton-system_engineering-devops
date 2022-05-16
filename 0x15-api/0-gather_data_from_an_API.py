#!/usr/bin/python3
""" REST Api """
import sys
import requests


if __name__ == '__main__':
    id = str(sys.argv[1])

    u_url = "https://jsonplaceholder.typicode.com/users/{}/todo".format(id)
    t_url = "https://jsonplaceholder.typicode.com/users/{}".format(id)
    user = requests.get(u_url).json()
	 todo = requests.get(t_url).json()
	 print(user.json())
	 print(todo.json())
