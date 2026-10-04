@echo off
echo ==============================================
echo   Compilation Fractales OpenGL (Onyx Engine)
echo ==============================================
g++ -O3 main.cpp Shader.cpp -o fractale.exe -Iinclude -Llib -DGLEW_STATIC -lglew32s -lglfw3 -lopengl32 -lgdi32
if %errorlevel% neq 0 (
    echo [ERREUR] Echec de la compilation!
    pause
    exit /b %errorlevel%
)

echo [SUCCES] Compilation reussie! Lancement...
echo.
fractale.exe
