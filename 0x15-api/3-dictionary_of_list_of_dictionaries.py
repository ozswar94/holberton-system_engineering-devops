#!/usr/bin/python3
""" export json on json file """
import json
import requests
import sys


if __name__ == '__main__':
    u_url = "https://jsonplaceholder.typicode.com/users"
    t_url = "https://jsonplaceholder.typicode.com/todos"

    users = requests.get(u_url).json()
    todos = requests.get(t_url).json()
    t = {}

    for user in users:
        list_todos = []
        for todo in todos:
            if todo.get("userId") == user.get("id"):
                todo_dic = {"username": user.get("username"),
                            "task": todo.get("title"),
                            "completed": todo.get("completed")}
                list_todos.append(todo_dic)
        t[user.get("id")] = list_todos

    filename = "todo_all_employees.json"
    with open(filename, 'w') as f:
        json.dump(t, f)
