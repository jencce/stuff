#! /bin/bash
echo "Start"
sed -i '4 i auth        required      pam_tally2.so deny=3 unlock_time=5 even_deny_root root_unlock_time=10' /etc/pam.d/system-auth

sed -i '/pam_cracklib/c password    requisite     pam_cracklib.so try_first_pass retry=3 minlen=11 minclass=3 difok=8' /etc/pam.d/system-auth

echo "password done"

useradd audit > /dev/null 2>&1

cat > /etc/auditcrontab <<EOF
SHELL=/bin/bash
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root
HOME=/

55 * * * * aureport --input-logs >> /var/log/audit/audit.report
55 * * * * cp -f /var/log/audit/audit.log /var/log/audit/audit.log.backup
45 * * * * backupAuditLog.sh 192.168.200.121 /var/log/
EOF

crontab /etc/auditcrontab > /dev/null 2>&1
echo "crontab /etc/auditcrontab" >> /etc/rc.local

chown root:root backupAuditLog.sh > /dev/null 2>&1
chmod 755 backupAuditLog.sh > /dev/null 2>&1
cp backupAuditLog.sh /bin/ > /dev/null 2>&1

echo "audit done"

echo "ALL:ALL EXCEPT 192.168.0.0/255.255.0.0" >> /etc/hosts.deny

echo "export TMOUT=600" >> /etc/profile

echo "All Done"
