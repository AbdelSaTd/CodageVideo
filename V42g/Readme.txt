1- Compiler et tester l'encodeur et le décodeur fournis, observer la vidéo codée et décodée 

La vidéo codée est grise de taille légérement supérieure à la vidéo originale.
Cela à cause de l'encodage des vecteurs de mouvement !?

La vidéo décodée est identique en taille et en forme à la vidéo originale.


*****************************************************************************************


2- Ajouter l'estimation de mouvement à l'encodeur
Attention: le bloc prédicteur ne doit jamais déborder des limites de l'image de référence

La fonction encode_avi s'occupe de découpé la vidéo en trames qu'elle passe à encode_trame. Plus précisément elle lui passe la trame courante et la trame précedente (avec un buffer vide à la première itération). Encode_trame quant à elle décompose chaque trame courante en bloc de 8x8. C'est là que nous intervenons pour réaliser l'estimation de mouvement. 
Dans le fichier encode.c, nous avons réalisé deux fonctions : 
- la fonction diff_bloc_estim() qui va déterminer la somme de la valeur absolue de la différence entre un bloc déterminé de l'image de précedente et le même bloc (au même emplacement) de l'image courante.

- la fonction estim_movement() qui va rechercher le bloc (dans la trame courante) qui minimise la variation d'amplitude des différences avec le bloc (dans la trame precedente) dont les coords sont fournies par encode_trame. Pour cela il va estimer la différence entre deux blocs, en appelant la fonction créée diff_bloc_estim(), en trouver le minimum par tous les blocs dans un espace de taille fixé par le paramètre breath puis en déduire le vecteur de mouvement associé.
 
On obtient alors, pour chaque image de la vidéo, des vecteurs de prédiction corrects par rapport à l'image précedente.

L'encodage des vecteurs de mouvement nous permet d'obtenir un meilleur taux de compression une meilleure compression de la vidéo.

- Quantification par défaut: 60% soit une augmentation du taux de compression de +30% en moyenne.

Cette amélioration est dûe au fait que l'on encode plus l'image entière (qui est relativement variable) mais seulement les vecteurs et la différence (qui est beaucoup plus monotone) entre l'image N et l'image N-1 sauf pour les images clés. Cela à donc pour effet d'augmenter le taux de compression des algorithmes.

*******************************************************************************************************************************************************************************************

3-Ajouter d'autres tables de quantification en vue d'améliorer la compressibilité de la video codée

Comme vous pourrez le voir dans le fichier readme.txt du dosssier Lab (V42g/Lab/readme.txt) nous avons experimenter 4 types de quantification si l'on ajoute la quantification par défaut qui nous a été fourni.

Nous avons effectués nos manips avec LZMA (Lempel–Ziv–MArkov) et ZIP qui sont deux algorithmes de compression sans pertes.

De manière globale nous avons pu observer (à partir du fichier V42g/Lab/analysis_file.txt) que le taux de compression augmente avec la quantification.

Cependant il est intéressant de noter qu'en quantifiant plus astucieusement (en quantifiant plus les mouvements de grandes amplitudes) comme c'est le cas avec la quantification à deux niveaux que nous avons bricolé, on pouvait avoir des rapports taux de compression/qualité de la vidéo bien plus intéressant.
A cela le logarithme n'est pas trop efficace puisqu'il croit rapidement et stagne après. La quantification est donc quasiment similaire entre les différences de moyennes amplitudes et celles de grandes amplitudes.

Dans le fichier (V42g/PLOT/v42_quant.html) vous pouvez observer les 4 types de quantification que nous avons utilisées modulo le changement de paramètre.

Commentaire de quelques valeurs :

Valeurs = taux de compression moyen obtenu

- Quantification par défaut: 60%
- Quantification à deux niveaux: Globalement cohérent
	- 4;128: 76%
	- 4;64: 76%
	- 2;128: 66%
	- 8;128: 84%


