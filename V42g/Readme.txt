1- Compiler et tester l'encodeur et le décodeur fournis, observer la vidéo codée et décodée 

La vidéo codée est grise de taille légérement supérieure à la vidéo originale. 
La vidéo décodée est identique en taille et en forme à la vidéo originale.


*****************************************************************************************


2- Ajouter l'estimation de mouvement à l'encodeur
Attention: le bloc prédicteur ne doit jamais déborder des limites de l'image de référence

Dans le fichier encode.c, nous avons réalisé deux fonctions : 
- la fonction diff_bloc_estim() qui va déterminer la somme de la valeur absolue de la différence entre le bloc de l'image de référence et le bloc de l'image source 

- la fonction estim_movement() qui va estimer la différence entre deux blocs, en appelant la fonction créée diff_bloc_estim(), en trouver le minimum de chaque valeur sum retournée par diff_bloc_estim() et déterminer le prédicteur de mouvement  

- Pour valider la prédiction, il suffit de lancer les deux commandes suivantes : 
//
// 

On obtient alors, pour chaque image de la vidéo, des vecteurs de prédiction corrects sur les images suivantes 

Les vecteurs de déplacement du prédiction nous permettent d'obtenir une meilleure compression de la vidéo 



1-bis Compression des images décodées avec mouvements et sans mouvements 

Compression avec quant 0 : 
Pour gunzip : 
- taux de compression du bus = image compressée avec mouvements / image compresssée originale = 6.4/5.8 = 1.1
- taux de compression de brandon = image compressée avec mouvements / image compressée originale = 15.3/15.3 = 1

Pour 7zip : 
- taux de compression du bus = image compressée avec mouvements / image compresssée originale = 5/4 = 1.2
- taux de compression de brandon = image compressée avec mouvements / image compressée originale = 12.5/12.2 = 1.02

Avec les deux compresseurs, la compression après encodage avec vecteurs de déplacement est meilleure que la compression originale. 
On remarque un meilleur taux de compression pour 7zip.

Cette amélioration est dûe au fait que l'on encode plus l'image entière mais seulment le vecteur de la différence entre l'image N et l'image N-1 sauf pour les images clés. 

*******************************************************************************************************************************************************************************************

3-Ajouter d'autres tables de quantification en vue d'améliorer la compressibilité de la video codée

quant 1 -> logarithme népérien 
quant 2 -> 8*logarithme népérien
quant 3 -> 16*logarithme népérien

Pour les résultats, voir TPCodage/V42g/PLOT/v42_quant.html

Commentaire des résultats : Le logarithme népérien coefficienté semble améliorer la compressibilité de la vidéo codée, ce qui va être vérifié par les taux de compression suivant

2-bis Compression des images décodées avec mouvements et sans mouvements 

Compression avec quant 1 : 
Pour gunzip : 
- taux de compression du bus = image compressée avec mouvements / image compresssée originale = 4.7/5.8 = 0.80
- taux de compression de brandon = image compressée avec mouvements / image compressée originale = 6.4/15.3 = 0.41

Pour 7zip : 
- taux de compression du bus = image compressée avec mouvements / image compresssée originale = 1.4/4 = 0.35
- taux de compression de brandon = image compressée avec mouvements / image compressée originale = 2.1/12.2 = 0.17


Compression avec quant 2 : 
Pour gunzip : 
- taux de compression du bus = image compressée avec mouvements / image compresssée originale = 4.7/5.8 = 0.80
- taux de compression de brandon = image compressée avec mouvements / image compressée originale = 6.4/15.3 = 0.41

Pour 7zip : 
- taux de compression du bus = image compressée avec mouvements / image compresssée originale = 1.4/4 = 0.35
- taux de compression de brandon = image compressée avec mouvements / image compressée originale = 2.1/12.2 = 0.17

Compression avec quant 3 : 
Pour gunzip : 
- taux de compression du bus = image compressée avec mouvements / image compresssée originale = 4.7/5.8 = 0.80
- taux de compression de brandon = image compressée avec mouvements / image compressée originale = 6.4/15.3 = 0.41

Pour 7zip : 
- taux de compression du bus = image compressée avec mouvements / image compresssée originale = 1.4/4 = 0.35
- taux de compression de brandon = image compressée avec mouvements / image compressée originale = 2.1/12.2 = 0.17