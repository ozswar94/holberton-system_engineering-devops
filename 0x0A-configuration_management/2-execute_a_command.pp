# execute file

exec { 'killmenow':
  command => 'pkill killmenow',
  path    => '/usr/local/bin/:/usr/bin:/bin/',
}
