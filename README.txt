-------------------
Description : Projet de création de poster photo automatique par optimisation

Auteur      : Caruyer Claire et Djama Wassim


-------------------
# Sommaire :
-  Liste des fichiers et dossiers
-  Création d'un poster
-  Commandes
    -  Fonction d'évaluation en c++
    -  Pour exécuter
-  Développement


-------------------
## Liste des fichiers et dossiers :

- code/generator/buildAlbum.py : code python permettant de créer les pages web avec l'album à partir d'un fichier de solution
- svg                          : dossier de sortir pour recevoir le poster au format svg
- svg/img/*.jpg                : les 55 photos au format jpg disponibles
- data/poster_*_0.json         : exemple de posters (instances du problème) de différentes tailles, conçus "à la main"
- data/instances               : dossier avec des instances aléatoires
- data/instances_param.csv     : fichier avec les paramètres des instances aléatoires
- data/solution_*.sol          : solution de disposition et de tailles des photos du poster (basic: par ordre chronologique et tailles égales)
- code/eval                    : dossier du code c++ de la fonction d'évaluation


-------------------
## Création d'un poster :

python code/generator/buildPoster.py out_in_directory poster_file_name_json photo_directory solution_file_name out_file_name

Par exemple:
python code/generator/buildPoster.py svg data/poster_48_0.json img data/solution-basic_48.sol poster.svg

Il faut ensuite ouvrir le fichier svg/poster.svg avec un nagivateur web.

-------------------
## Commandes :

### Fonction d'évaluation en c++ :

Pour compiler:

cd code/eval
mkdir build
cd build
cmake ..
make

### Pour exécuter :

./src/main ../../../data/poster_48_0.json

------------------
## Développement :

- Mise en place d'un algo hcBestImprovement