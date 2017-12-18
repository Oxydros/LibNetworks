Librairie C++ pour communiquer entre les applications CubZ

# Installation des dependances pour Windows 64 Bits
Boost:
- Télécharger la librairie Boost (http://www.boost.org/users/download/)
- Utiliser la commande
    ```b2.exe --toolset=msvc variant=release link=static threading=multi runtime-link=static address-model=64 stage```
- Definir la variable d'env BOOST_ROOT vers le dossier Boost (cf Google)

Protobuf:
- Suivre https://github.com/google/protobuf/blob/master/cmake/README.md

# Compilation de la librairie

Windows:
- Ouvrir le dossier à l'aide Visual Studio 2017

Linux:
- Executer le script build.sh
- Les libs se trouvent dans le dossier build/lib


# TODO
- Plus de detail sur la compilation via CMake sur Windows
- Plus de tests unitaires
- Ajouter l'installation des dependances directement depuis le CMake