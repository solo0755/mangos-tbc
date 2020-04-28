#!/bin/sh
#mangosSqlName=$1
cmd="mysql -uroot -proot -s --default-character-set=utf8"
cmdimport="mysql -uroot -proot -s --default-character-set=utf8"
${cmd} -e "set global max_allowed_packet=2684354560"

${cmd} tbcmangos < init.sql
${cmd} tbcmangos < creature.sql
${cmd} tbcmangos < npc_trainer.sql
${cmd} tbcmangos < npc_vendor.sql
