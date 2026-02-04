# Liste des ajouts fait après le jours de la validation

- gestion de signe négatif retirer de manualPilot/analysis.c
- ajout de commentaires doxygen
- amélioration du script `visualize.py` dans le module vision
- ajustement de l'artifactThreshold dans le fichier `vision.c` afin d'améliorer les performances
- ajustement du fichier makefile afin de permettre la compilation de la cible PFR-unit même sans Xmake
- placement des deux hyperparamètres du module de vision dans le fichier `visionConfig.toml`. les autres const seront ajouter lors du PFR2