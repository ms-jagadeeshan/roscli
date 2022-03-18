#!/bin/bash

# Build and sources the workspace
alias basows="catkin build -w \`catkin locate\` && source \`catkin locate -d\`/setup.bash"

# sources the workspace
alias sows="source \`catkin locate -d\`/setup.bash"

# cd to catkin pkg
alias procd='cd $(catkin locate $(catkin list -u | fzf ))'

# finds all workspace and sources the selected workspace
# dependency fzf
function prows() {
    cd $(find ${HOME} -maxdepth 6 -path '**/src/*' -name 'CMakeLists.txt' -type l 2>/dev/null | awk -F"/src" '{print $1}' | fzf) && test -f devel/setup.bash && source devel/setup.bash
}

## Syntax: set_master_uri <ip::port>
## Eg: set_master_uri                  - sets master uri as http://localhost:11311
##     set_master_uri 12.24.2.5        - sets master uri as http://12.24.2.5:11311
## And sets ROS_IP to its ip
function set_master_uri() {
    local ROS_BASH="${HOME}/.ros.bash"
    sed -i --follow-symlinks "s/^export ROS_MASTER_URI=.*/export ROS_MASTER_URI=http:\/\/${1:-localhost}:11311/" "${ROS_BASH}"
    local ip
    ip=$(hostname -I | cut -d ' ' -f 1)
    if [[ -n "${1}" ]]; then
        sed -i --follow-symlinks "s/^#export ROS_IP=.*/export ROS_IP=${ip}/" "${ROS_BASH}"
        sed -i --follow-symlinks "s/^export ROS_IP=.*/export ROS_IP=${ip}/" "${ROS_BASH}"
    else
        sed -i --follow-symlinks "s/^export ROS_IP=.*/#export ROS_IP=/" "${ROS_BASH}"
    fi
}

export ROS_MASTER_URI=http://localhost:11311
#export ROS_IP=
