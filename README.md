<a name="readme-top"></a>

<!-- PROJECT HEADER -->
<div align="center">
    <h3 align="center">Projet ELE4205</h3>
    <p align="center">
        Groupe 01 - Équipe 02
    <p>
    <br>
    <p align="center">
        Barri, Alfa Umar
    <p>
    <p align="center">
        Désilets, Emmanuel 
    <p>
<div>

<!-- TABLE OF CONTENTS -->
<h5>Table des matières</h5>
<ol>
	<li><p>À propos du projet</p></li>
	<li><p>Comment commencer</p></li>
	<li><p>Documentation</p></li>
	<li><p>Hardware</p></li>
	<li><p>Commandes SSH avec l'odroid</p></li>
	<li><p>Manipulations git</p></li>
	<li><p>Messages et signaux Client/Serveur</p></li>
	<li><p>Signal PUSHB du serveur</p></li>
	<li><p>Detection/Reconnaissance faciale</p></li>
	<li><p>Notes supplémentaires</p></li>
	<li><p>Remerciement</p></li>
</ol>



<!-- À PROPOS DU PROJET -->
<h5>À propos du projet</h5> 
<p>Ceci est un projet d'école réalisé par des étudiants de dernière année en génie électrique de l'École Polytechnique Montréal. Ce projet à été fait dans le cadre du cours Système d'exploitation et interfaces matérielles (sigle ELE4205) pendant la session d'automne 2023.</p>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- COMMENT COMMENCER -->
<h5>Comment commencer</h5>
<p>Insérer la carte SD configuré lors des laboratoires de la session dans l'odroid à votre disposition.</p>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- DOCUMENTATION -->
<h5>Documentation</h5>
<p>Veuillez consulter le DOxygen du projet pour consulter la documentation des classes, fonctions et variables du projet.</p>
<p>Quelques précisions par rapport au projet seront présentes dans les sections suivantes.</p>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- HARDWARE -->
<h5>Hardware</h5>
<p>Dans ce projet, nous avons utilise un odroid-c2 qui fonctionne sous une architecture aarch64 comme serveur et un ordinateur Antec
sous une architecture x86 comme client. Sur le client, le systeme d'exploitation etait une distribution Linux appelee Centos7. Sur le serveur,
le systeme d'exploitation etait minimal et construit a partir d'une image d'un projet Yocto. Nous pouvions compiler les programmes pour le 
serveur en effectuant de la compilation croise. Avant de faire les commandes [cmake ..] et [make], nous devions aller dans le terminal bash
[bash] et faire cette commande [source /usr/local/opt/poky/2.1.3/environment-setup-aarch64-poky-linux], pour configurer les variables d'environnements.</p>
<p>Pour capturer les images, une camera USB Logitech c270 etait utilise. De plus, une carte SD devra etre utilise pour le systeme d'exploitation du server.</p>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- COMMANDES SSH AVEC L'ODROID -->
<h5>Commandes SSH avec l'odroid</h5>
<p>Voici quelques commandes ssh utiles:</p>
<ol>
  <li><p>sudo /users/Cours/ele4205/commun/scripts/ifconfig-enp0s-up</p></li>
  <p>Activer le réseau.</p>

  <li><p>ssh root@192.168.7.2</p></li>
  <p>Accès au terminal de l'odroid.</p>

  <li><p>scp -r [ficher ou dossier à copier] root@192.168.7.2:[répertoire où on fait la copie dans l'odroid]</p></li>
  <p>Copie d'un fichier ou dossier vers l'odroid.</p>
</ol>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MANIPULATION GIT -->
<h5>Manipulations git</h5>
<p>Pour ce projet, nous avions un répertoire bitbucket que nous pouvions manipuler avec des commandes git.</p>
<p>Voici quelques commandes git qui nous ont été utile lors du projet:</p>
<ol>
  <li><p>git clone [url du projet]</p></li>
  <p>Copie le projet du server où il est situé au répertoire courant. Cette commande initialise le répertoire comment un répertoire git.</p>

  <li><p>git status</p></li>
  <p>Cette commande est la plus importante! Elle nous donne l'état de tous les fichiers suivis et non suivis. Cette commande nous permet d'avoir une vu d'ensemble des changements effectués depuis le dernier commit.</p>

  <li><p>git pull [branche] [repositoire]</p></li>
  <p>Cette commande met à jour le projet du répertoire courant avec le projet de la branche spécifiée du répertoire spécifié sur le serveur.</p>
  
  <li><p>git add [fichier ou dossier à ajouter]</p></li>
  <p>Ajoute les fichiers spécifiés à la liste des fichiers suivis.</p>

  <li><p>git restore [fichier ou dossier à ajouter]</p></li>
  <p>Enlève les changements apportés aux fichiers non suivis spécifiés.</p>

  <li><p>git restore --staged [fichier ou dossier à ajouter]</p></li>
  <p>Enlève les fichiers suivis spécifés de la liste des fichiers suivis.</p>

  <li><p>git commit -m [message du commit]</p></li>
  <p>Enregistre tous les changements des fichiers suivis sous la forme d'un commit.</p>

  <li><p>git push [branche] [repositoire]</p></li>
  <p>Mets à jour le projet de la branche spécifiée du répertoire spécifié sur le serveur avec le projet local.</p>

  <li><p>git tag</p></li>
  <p>Liste tous les tags.</p>

  <li><p>git tag [nom du tag] [numéro de commit]</p></li>
  <p>Liste tous les tags.</p>

  <li><p>git log</p></li>
  <p?>Historique des commits.</p>
</ol>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MESSAGE ET SIGNAUX CLIENT/SERVEUR-->
<h5>Messages et signaux Client/Serveur</h5>
<p>Les instances des classes Server et Client auront chacun un attribut msg qui représente le message échangé
entre le client et le serveur. Ce message est un octet dont les bits ou groupe de bits représentent un signal 
(Par exemple, le premier bit veut dire ELE4205_OK). Voici une liste des messages et leur signification.</p>
<br>
<ol>
  <li><h6>Bit 0</h6></li>
  <p>Ce bit représente la volonté du client à garder la communication.</p>
  <ol>
    <li><p>0 - ELE4205_OK</p></li>
    <li><p>1 - !ELE4205_OK</p></li>
  </ol>
  
  <li><h6>Bit 1</h6></li>
  <p>Ce bit représente le format de l'image choisi par l'usager.</p>
  <ol>
    <li><p>0 - .jpg</p></li>
    <li><p>1 - .png</p></li>
  </ol>

  <li><h6>Bit 2 et 3</h6></li>
  <p>Ces bits représente la résolution de l'image choisi par l'usager.</p>
  <ol>
    <li><p>00 ou 0 - 176x144</p></li>
    <li><p>01 ou 1 - 320x240</p></li>
    <li><p>10 ou 2 - 800x600</p></li>
    <li><p>11 ou 3 - 960x720</p></li>
  </ol>
  
  <li><h6>Bit 4 et 5</h6></li>
  <p>Ces bits représente le message du serveur au client.</p>
  <ol>
    <li><p>00 ou 0 - IDOWN</p></li>
    <li><p>01 ou 1 - READY</p></li>
    <li><p>10 ou 2 - PUSHB</p></li>
  </ol>

  <li><h6>Bit 6 et 7</h6></li>
  <p>Ces bits représente l'état de la reconnaissance faciale.</p>
  <ol>
    <li><p>00 ou 0 - LEARN</p></li>
    <li><p>01 ou 1 - KNOWN</p></li>
    <li><p>10 ou 2 - UNKNOWN</p></li>
  </ol>
</ol>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- Signal PUSHB du serveur -->
<h5>Signal PUSHB du serveur</h5>
<p>Voici quelques precisions par rapport a la principale fonctionnalite du projet.</p>
<p>Lorsque l'usager pese sur le bouton physique du cote serveur, le premier serveur envoie un signal de PUSHB au client, fait un fork() et demarre un executable qui est le deuxieme serveur. Ce deuxieme serveur attend une connection du client. Lorsque le client recoit le signal PUSHB, il fait un fork() et demarre la reconnaissance faciale sur la derniere image recue. Apres avoir traite l'image, le client se connecte au deuxieme serveur, envoie le resultat de son analyse et ferme son programme parrallel. Le programme parralel du deuxieme serveur se ferme egalement apres avoir joue un son s'il y a lieu.</p>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- Detection/Reconnaissance faciale -->
<h5>Detection/Reconnaissance faciale</h5>
<p>Voici quelques precisions par rapport a la detection et reconnaissance faciale.</p>
<ol>
    <li><p>Lors du demarrage du programme, veuillez suivre les instructions dans le terminal.</p></li>
    <li><p>En LEARNING MODE, entrez votre nom et rester seul et face a la camera. Une indication de la progression de l'entrainement sera disponible sur le terminal. A la fin de l'entrainement, les commandes disponibles seront affichee sur le terminal.</p></li>
    <li><p>En RECOGNIZATION MODE, pesez sur le bouton PUSHB pour faire la reconnaissance faciale des visages qui apparaissent dans la diffusion video. Le terminal affichera les resultats de l'analyse. De plus, une image sera capturees avec les visages detectes sera disponible dans le dossier pictures du cote client. Si le dossier n'existe pas, creez le.</p></li>
 </ol>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- NOTES SUPPLÉMENTAIRES -->
<h5>Notes supplémentaires</h5>
<p>Dans le repertoire du client et le serveur, il faut creer le dossier build ou il y aura toute la compilation et les programmes compiles.</p>
<p>Pour faire la compilation, il faut se retrouver dans le dossier build et faire les commandes suivantes [cmake .. && make]</p>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- REMERCIEMENT -->
<h5>Remerciement</h5>
<p>Nous aimerions remercier le professeur et les chargés du cours.</p>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>
