#fix error nginx server

exec { 'fix error':
    path    => '/usr/local/bin/:/bin/',
    command => 'sed -i "s/worker_processes 4;/worker_processes 8;/g" /etc/nginx/nginx.conf; sudo service nginx restart'
}
