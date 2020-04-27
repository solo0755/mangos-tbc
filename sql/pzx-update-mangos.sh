#!/bin/sh
#mangosSqlName=$1
cmd="mysql -uroot -proot -s --default-character-set=utf8"
cmdimport="mysql -uroot -proot -s --default-character-set=utf8"

#创建库
printf "删除再创建库 \n"
${cmd} -e "DROP DATABASE IF EXISTS tbcmangos";
${cmd} -e "CREATE DATABASE tbcmangos DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci";
printf "初始化mangos数据库 \n"
${cmd} --database=tbcmangos < base/mangos.sql
printf "初始化dbc数据2 \n"
for sql_file in $(ls base/dbc/original_data/*.sql); do ${cmd} --database=tbcmangos < $sql_file ; done
printf "初始化dbc数据3 \n"
for sql_file in $(ls base/dbc/cmangos_fixes/*.sql); do ${cmd} --database=tbcmangos < $sql_file ; done
