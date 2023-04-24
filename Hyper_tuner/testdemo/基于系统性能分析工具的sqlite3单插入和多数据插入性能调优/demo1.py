import sqlite3
import os


if os.path.exists('sqlite_name.bd'):
    os.remove('sqlite_name.bd')
conn=sqlite3.connect('sqlite_name.bd')
cu=conn.cursor()
try:
    cu.execute("""create table usermanager_user
(
  id            integer      not null
    primary key autoincrement,
  password      varchar(96) not null,
  username      varchar(64)  not null
    unique,
  role          varchar(16)   not null,
  is_firstlogin bool         not null,
  time_created  varchar(64)    not null,
  last_login    varchar(64)     not null,
  time_pwreset  varchar(64)     not null,
  token         varchar(255) not null,
  exp_time      varchar(255) not null
);""")
except:
    print('表已存在')


for i in range(1, 2000000):
    print(i)
    cu.execute("insert into usermanager_user (id, password, username, role, is_firstlogin, time_created, last_login, time_pwreset, token, exp_time) values (?,?,?, ?,?,?,?, ?,?,?)", (i, 'FJDSLAJFWEQEJWfkndsajrek3w24wdjaksljdlkjepqfwbfdsafhdjksaj234j', 'name'+str(i), 'name', True, '2020-07-02 19:09:04.412260', '2020-07-02 19:09:04.412260', '2020-07-02 19:09:04.412260', 'JWT fhdsjah1FG2321kjkfdhasjKJHG232fjsdafke3HGFD2423dewqjro23herkj32KHdnweqjhr3kj21h4k3GF2dkbewkjqer3H2k1h32dnHnewq', '2020-07-02 19:09:04.412260'))
    if i % 1000 == 0:
        conn.commit()

conn.close()
