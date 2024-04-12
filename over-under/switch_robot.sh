if [ $# -ne 1 ]; then
    echo "Usage: $0 {drew,tanner}"
    exit 1
fi


if [ $1 = 'drew' ] || [ $1 = 'tanner' ]; then
    if [ $1 == 'drew' ]; then opposite='tanner'; fi
    if [ $1 == 'tanner' ]; then opposite='drew'; fi
    python change_program_name.py $1 
    sed s/global_defs_${opposite}.h/global_defs_${1}.h/g -i src/*
    mv src/global_defs_${opposite}.cpp .
    mv global_defs_$1.cpp src
#    sed /moveDriveTanner/moveDriveDrew.h
fi