@echo off
IF "%1"=="-h" GOTO :help

pushd ..\
call git subtree split --prefix=GitGud --branch engine

IF "%1"=="" GOTO :with_no_tag
IF NOT "%1"=="" GOTO :with_tag

GOTO :end

:with_tag
    call git tag %1 engine
    call git push origin engine --tags
    GOTO :end

:with_no_tag
    call git push origin engine
    GOTO :end

:help
    echo   No arguments split the engine and push it.
    echo   First argument specifies the tag.
    GOTO :eof

:end
    popd
    PAUSE
