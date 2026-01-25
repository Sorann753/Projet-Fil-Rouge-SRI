# Projet-Fil-Rouge-SRI
Le projet fil rouge de notre première année d'école d'ingénieur en robotique

## Assurance qualitée
Ce repository utilise Lizard analysis pour analyser la qualité du code d'un point de vue de la lisibilité et maintenabilité.

D'autres outils de vérifications seront sans doute ajoutés plus tard tel que clang-tidy et cppcheck

## Compilation et execution

* Avec makefile

 ```bash
  make PFR-test
  make run-test
  ```

* Avec xmake

 ```bash
  xmake -b PFR-test
  xmake r PFR-test
  ```
## Étapes pour créer l’environnement Python pour l'interface vocale

### 1. Installer Miniconda

* Télécharger le script d’installation de Miniconda (Linux)

  ```bash
  bash Miniconda3-latest-Linux-x86_64.sh
  ```
* Suivre les instructions affichées à l’écran.
* Accepter l’ajout de Miniconda au PATH lorsque demandé.

### 2. Ouvrir un terminal

### 3. Se placer dans le dossier contenant le fichier `environment.yml`

```bash
cd <votre_chemin>/Projet-Fil-Rouge-SRI/requirements/
```

### 4. Créer l’environnement conda

```bash
conda env create -f environment.yml
```

### 5. Activer l’environnement

```bash
conda activate PROJET_PFR
```

### 6. Vérifier que tout fonctionne

```bash
conda list
python --version
```


