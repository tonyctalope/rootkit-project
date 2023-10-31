# Instructions d'installation

Suivez ces étapes pour installer et lancer le module :

## 1. Configuration du script
Modifiez la valeur de `LINUX_PATH` dans `script.sh` et dans les Makefile des modules:
```bash
LINUX_PATH=../linux-5.15.137
```
Ajustez-la en fonction de l'emplacement réel de votre Linux.

## 2. Compilation de Linux et du module

Avant de continuer, assurez-vous de compiler correctement le noyau Linux et le module :

```bash
make defconfig && make
```

Puis, dans le dossier du module, exécutez :

```bash
make
```

## 3. Lancement du script

Exécutez le script avec la commande suivante :

```bash
./script.sh
```

## 4. Installation du module dans QEMU

Une fois dans l'environnement QEMU, accédez au répertoire des modules et installez votre module :

```bash
cd /lib/modules && insmod [nom_du_module]
```

Remplacez `[nom_du_module]` par le nom réel de votre module.

# Fonctionnalités

-  Cache n'importe quel fichier/dossier qui contient "hacking2600Module" (getdents64)