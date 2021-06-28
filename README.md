# Shell

![Release](https://img.shields.io/badge/Release-v1.0-blueviolet)
![Language](https://img.shields.io/badge/Language-C%2B%2B-0052cf)
![size](https://img.shields.io/badge/Size-19Ko-f12222)
![Open Source](https://badges.frapsoft.com/os/v2/open-source.svg?v=103)

<br/>

Ce dépôt contient le code source d'un shell simplifié.

<br/>

<p align="center">
	<img src="https://i.imgur.com/JxZfN02.png" width="100%">
</p>

<br/>

# Fonctionnalités

* Le lancement de programmes :
  * Les programmes de base (exemples : `pwd`, `ls`, `cat`, etc...).
  * N'importe quel programme avec `path/name` (exemple : `./a.out`).
  * Builtins : `cd`, `cp` et `mkdir`.

<br/>

* Les redirections d'entrée et de sortie de processus avec `< filename`, `> filename` ou `>> filename` (exemple : `echo bonjour > test.txt`).

<br/>

* Les pipes avec `|` (exemple : `printf "A\nB\nC" | grep B | cat`)

<br/>

# Remarques

* Le programme ne fonctionne que sur un environnement Unix.

* Ce programme est un projet scolaire pour Polytech Paris-Saclay dans le cadre du cours de Système.

<br/>

# Crédits

* [**Angel Uriot**](https://github.com/angeluriot) : Co-créateur du projet.
* [**Arthur Azambre**](https://github.com/arthurazambre) : Co-créateur du projet.
