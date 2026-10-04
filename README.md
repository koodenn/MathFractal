<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3%2B-5586A4?style=for-the-badge&logo=opengl&logoColor=white)
![GLSL](https://img.shields.io/badge/GLSL-330_Core-green?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Windows%20x64-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-purple?style=for-the-badge)

<p align="center">
  <b>Explorateur de fractales interactif en temps réel accéléré par le GPU (shaders GLSL 3.30 Core).</b><br>
  Rendu 60+ FPS, lissage continu du potentiel, palettes procédurales vibrantes et navigation fluide à la souris.
</p>

</div>

---

## Contrôles & Raccourcis

| Raccourci | Action |
|---|---|
| **Clic Gauche + Glisser** | Déplacer la caméra (*Pan*) |
| **Molette Souris** | Zoom avant / arrière centré sur le curseur |
| `1` / `2` / `3` / `4` | Basculer entre les fractales (*1: Julia, 2: Mandelbrot, 3: Burning Ship, 4: Tricorn*) |
| `C` | Changer de palette de couleurs (5 thèmes) |
| `Clic Droit` ou `M` | Activer / Désactiver le contrôle interactif de la constante de Julia à la souris |
| `ESPACE` | Mettre en pause ou relancer l'animation temporelle |
| `R` | Réinitialiser la vue et le zoom par défaut |
| `Flèches HAUT / BAS` | Ajuster la précision (itérations max) |
| `H` | Afficher l'aide des raccourcis dans la console |
| `ÉCHAP` | Quitter l'application |

---

## Architecture du Projet

```text
FractaleGen/
├── fragment.glsl       # Shader de calcul des fractales & palettes de couleurs GPU
├── vertex.glsl         # Vertex shader projetant le quad plein écran
├── main.cpp            # Boucle OpenGL, gestion du contexte GLFW & entrées utilisateur
├── Shader.h / .cpp     # Wrapper C++ pour le chargement & la compilation des shaders
├── run.bat             # Script de build et lancement en un clic (MinGW)
├── include/            # En-têtes OpenGL (GLEW, GLFW)
├── lib/                # Bibliothèques statiques x64 (glew32s, glfw3)
```

---

## Installation & Lancement

### Prérequis
- **Windows x64**
- **MinGW-w64 (g++)** disponible dans votre `PATH` (ex: via MSYS2 ou MinGW standalone).

### Lancement Rapide
```bat
.\run.bat
```

### Compilation Manuelle
```bash
g++ -O3 main.cpp Shader.cpp -o fractale.exe -Iinclude -Llib -DGLEW_STATIC -lglew32s -lglfw3 -lopengl32 -lgdi32
.\fractale.exe
```

---

<div align="center">
  <sub>Fait avec passion pour l'infographie et les mathématiques.</sub>
</div>
