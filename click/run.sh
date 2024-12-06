#!/bin/sh

UNIT_FILE=$HOME/.config/systemd/user/exphoned.service

if [ ! -f "$UNIT_FILE" ]; then

    echo "Creating service $UNIT_FILE"
    mkdir -p "$(dirname "$UNIT_FILE")"
    cat <<"EOF" > $UNIT_FILE
[Unit]
Description=exPhone daemon
After=graphical.target

[Service]
ExecStart=/opt/click.ubuntu.com/com.github.jmlich.exphone/current/bin/exphoned
Restart=always
RestartSec=5
Environment=LD_LIBRARY_PATH=/opt/click.ubuntu.com/com.github.jmlich.exphone/current/lib:/opt/click.ubuntu.com/com.github.jmlich.exphone/current/lib/aarch64-linux-gnu:$LD_LIBRARY_PATH
Environment=HOME=%h XDG_CONFIG_HOME=/home/%u/.config DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/%U/bus XDG_RUNTIME_DIR=/run/user/%U

[Install]
WantedBy=default.target

EOF

    /usr/bin/systemctl daemon-reload
    /usr/bin/systemctl --user enable exphoned.service
    /usr/bin/systemctl --user start exphoned.service

else

    # restart just after reinstall

    mod_time=$(stat --format="%Y" ./bin/exphoned)
    enter_active=$(date -d "$(systemctl show -p ActiveEnterTimestamp --value --user exphoned)" +%s) #"
    if [ "$mod_time" -gt "$enter_active" ]; then
        /usr/bin/systemctl --user restart exphoned.service
    fi

fi

export LD_LIBRARY_PATH=$PWD/lib:$PWD/lib/aarch64-linux-gnu:$LD_LIBRARY_PATH
export QT_QUICK_CONTROLS_MOBILE=true
export QT_QUICK_CONTROLS_STYLE=Suru

exec $PWD/bin/exphone \
    --desktop_file_hint=${HOME}/.local/share/applications/${APP_ID}.desktop "$@"
