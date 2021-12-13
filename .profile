# Set prompt to be colorful and in the format of 'user@dockerdev:/dir$ '
PS1='\[\033[01;32m\]\u@dockerdev\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '

# Enable colors for ls and grep
alias ls='ls --color=auto'
alias grep='grep --color=auto'

# Options for Valgrind
export VALGRIND=$HOME/valgrind
export PATH="$VALGRIND/bin:$PATH"
export VALGRIND_LIB=$VALGRIND/libexec/valgrind
