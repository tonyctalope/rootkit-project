# Développement d'un RootKit Basique pour Environnements Linux

## Objectif
Ce document présente le développement d'un RootKit basique destiné aux systèmes Linux, en mettant l'accent sur la dissimulation, la modification de privilèges, et la persistance.

## Prérequis
- **Docker**: En mode utilisateur (ajoutez `sudo` dans `script.sh` sinon).
- **Packages APT**: `flex`, `bison` (pour le build du kernel Linux).
- **Grub i386**: Pour l'émulation de VM Linux.

## Emulation d'une VM Linux
Pour lancer l'émulation, exécutez `script.sh`. Ce script configure et démarre une machine virtuelle Linux pour le test du RootKit.
Deux utilisateurs sont prévus pour l'exécution de test : 
- Un utilisateur admin (root:root)
- Un utilisateur (test:test)

## Installation du RootKit
Dans la VM, exécutez `loadModule.sh` pour installer le module du RootKit.

## Fonctionnalités
- **Dissimulation de Fichiers/Dossiers**: Cache les éléments contenant "hacking2600Module" via le hook `getdents64`.
- **Masquage dans lsmod et /proc/modules**: Rend le module invisible dans les listes de modules.
- **Modification des Privilèges**: Utilise `kill -64 1` pour attribuer des permissions root.
- **Persistance**: Implémentée via `initd` (openrc) pour une meilleure dissimulation.

## Sécurité et Utilisation Éthique
Ce RootKit est développé à des fins éducatives. Son utilisation doit respecter les lois et réglementations en vigueur. Les développeurs ne sont pas responsables de l'utilisation malveillante de ce logiciel.

## Améliorations Futures et Limitations
- **Limitations**: La principale limitation actuelle est le nombre limité de fonctionnalités disponibles dans ce RootKit.
- **Améliorations**: Une amélioration envisageable serait d'ajouter une connexion socket vers l'extérieur avec des droits administrateur, permettant une interaction et un contrôle à distance plus sophistiqués.