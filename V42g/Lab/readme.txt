Notation des quantifications

Cste{n} : n = {8, 16, 32} 
Quantification uniforme croissante. On divise l'espace des différences (abscices - x) en groupe de n valeurs
Plus n est grand et plus la quantification est importante

{n}log : n = {4, 8, 16}
Quantification logarithmique avec costante multiplicative

Default
Quantification par défaut donner au début du TP. Elle est similaire à la quantification constante

DoubleStep{n}-{m}
Quantification à deux niveaux. On divise l'espace des différences (abscice -x) en deux et 
on y applique pour chacun une quantification constance differente
{n} : represente la quantification sur la première partie (les petites différences)
{m} : represente la quantification sur la deuxième partie (les grandes différences)
L'intérêt de cette quantification réside dans le fait qu'on a plus intérêt à quantifier les grandes différences
qui se caractérisent à l'écran par des mouvements de grandes amplitudes (rapides) sur lesquels l'oeil percevrait 
moins la dégradation liés à la quantification que par rapport aux mouvements quasi statiques.


CompRate: désigne le taux de compréssion nette obtenu par l'algorithme sur le fichier encodé. C'est-à-dire le rapport entre la diminution de taille et la taille d'origine.

x: le deuxième chiffre (par exemple; either x compression increase) représente la différence entre le taux de compréssion obtenu après encodage (CompRate) et le taux de compression nette de la vidéo orginale (sans encodage). Il caractérise donc le gain ou la perte de compressibilité de l'encodage (donc de la quantification)

