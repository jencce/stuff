#! /bin/bash

cat > /tmp/auditcrontab <<EOF
55 * * * * aureport --input-logs >> /var/log/audit/audit.report
55 * * * * cp -f /var/log/audit/audit.log /var/log/audit/audit.log.backup
45 * * * * backupAuditLog.sh 192.168.200.121 /var/log/
EOF
