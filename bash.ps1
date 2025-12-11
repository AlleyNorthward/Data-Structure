function Invoke-Project{
    # 不要写复杂脚本. 写灵活性强的. 因为你不知道接下来存在多少种变化的可能性, 要是仍想兼容, 那么修改脚本也会成为一种复杂的事. 可是脚本本来就是为了简化操作的, 如果太复杂, 又有什么存在的必要呢? 
    param(
        [Alias("N")]
        [string]$Name = "Main",

        [Alias("M")]
        [ValidateSet("Debug", "Release", "Update", "Delete")]
        [string]$Mode = "Release",

        [Alias("C")]
        [ValidateSet("C", "CXX", "C_CXX")]
        [string]$Compiler = "C"
    )
    if (-not (Test-Path build)) {
        New-Item -ItemType Directory build | Out-Null 
    }

    cd build
    if($Mode -eq "Update"){
        if($Compiler -eq "C"){
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                ..
        }
        elseif($Compiler -eq "CXX"){
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        else{
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        move-item .\compile_commands.json ..\ -force
        cd..
    }
    elseif($Mode -eq "Release"){
        if($Compiler -eq "C"){
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                ..
        }
        elseif($Compiler -eq "CXX"){
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        else{
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        cmake --build .
        move-item .\compile_commands.json ..\ -force
#         & .\$Name.exe
        cd..
    }
    elseif($Mode -eq "Debug"){
        if($Compiler -eq "C"){
            cmake `
                -DCMAKE_BUILD_TYPE=Debug `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                ..
        }
        elseif($Compiler -eq "CXX"){
            cmake `
                -DCMAKE_BUILD_TYPE=Debug `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        else{
            cmake `
                -DCMAKE_BUILD_TYPE=Debug `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        cmake --build .
        move-item .\compile_commands.json ..\ -force
#         & gdb .\$Name.exe 
        cd..
    }
    else{
        cd ..
        if (Test-Path build) {
            Remove-Item build -Recurse -Force
        }

        if (Test-Path compile_commands.json) {
            Remove-Item compile_commands.json -Force
        }

        if (Test-Path .cache) {
            Remove-Item .cache -Recurse -Force
        }
    }
}

function Invoke-Build{
    param(
        [Alias("M")]
        [ValidateSet("ON", "OFF")]
        [string]$Mode = "OFF"
    )
    if($Mode -eq "ON"){
        cmake -S . -B build -DBUILD_QUESTIONS=ON
    }
    else{
        cmake -S . -B build -DBUILD_QUESTIONS=OFF
    }

    cd build
    if (Test-Path compile_commands.json) {
        move-item .\compile_commands.json ..\ -force
    }
    cd ..
}

function Invoke-Run{
    cmake --build build --config Release
    cd build
    .\main.exe
    cd ..
}

function Invoke-Delete{
    if (Test-Path build) {
        Remove-Item build -Recurse -Force
    }
#     if (Test-Path compile_commands.json) {
#         Remove-Item compile_commands.json -Force
#     }

    if (Test-Path .cache) {
        Remove-Item .cache -Recurse -Force
    }
}
