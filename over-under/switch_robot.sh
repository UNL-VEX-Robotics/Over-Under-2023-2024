if [ $# -ne 1 ]; then
    echo "Usage: $0 {drew,tanner}"
    exit 1
fi

if [ $1 == 'drew' ]; then
    python change_program_name.py drew
    #sed /global_defs_tanner.h/global_defs_drew.h
    #sed /moveDriveTanner/moveDriveDrew.h
fi