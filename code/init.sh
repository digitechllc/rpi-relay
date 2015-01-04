#!/bin/bash
#
# This starts and stops shairport
#
### BEGIN INIT INFO
# Provides:          shairport
# Required-Start:    $network avahi
# Required-Stop:
# Short-Description: shairport - Airtunes emulator!
# Description:       Airtunes emulator!
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
### END INIT INFO


# Source function library.
. /lib/lsb/init-functions

NAME=shairport
DAEMON="/usr/local/bin/shairport"
PIDFILE="/var/run/$NAME.pid"
LOGFILE="/var/log/shairport.log"
DAEMON_ARGS="-d -P $PIDFILE -l $LOGFILE -e $LOGFILE -b 64 -a Kitchen -B /usr/local/bin/amp-on -E /usr/local/bin/amp-off"

[ -x $binary ] || exit 0

RETVAL=0

start() {
    log_daemon_msg "Starting shairport" "shairport" || true
    start-stop-daemon --start --quiet --pidfile "$PIDFILE" --exec "$DAEMON" -b --oknodo -- $DAEMON_ARGS
    log_end_msg $? || true
}

stop() {
    log_daemon_msg "Shutting down shairport" "shairport" || true
    start-stop-daemon --stop --quiet --pidfile "$PIDFILE" --retry 1 --oknodo
    log_end_msg $? || true
}

restart() {
    stop
    sleep 1
    start
}

case "$1" in
    start)
        start
    ;;
    stop)
        stop
    ;;
    status)
        status shairport
    ;;
    restart)
        restart
    ;;
    *)
        echo "Usage: $0 {start|stop|status|restart}"
    ;;
esac

exit 0

