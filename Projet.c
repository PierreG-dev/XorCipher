#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


/* * * * * * * * * * * * * * * * *
* GODINO Pierre TDA 5.1          *
* 21606179                       *
* * * * * * * * * * * * * * * * */


// Variables globales
unsigned char local_dico[] = {"dictionnaire.txt"};
long nombre_mots_dico = 0; //variable servant a la fonction de lecture du dictionnaire
long cles_tab_total = 0; // Variable représentant le nombre de clés possibles apres verification de C1, variable servant notemment dans la fonction "Concatenation_main" et sa sous fonction "Fonction_for"
int Taille_text_in = 0; // Taille du texte d'entree (en nombre de char)
int Text_in_compt = 0, Text_out_compt = 0, Key_compt = 0, Lengh_key_compt = 0, Mode_compt = 0;
unsigned const char *Text_in, *Text_out, *Key;
const char *Mode;
int Lengh_key;
int total_args = 0; // total des arguments


//******************************************************************************
//--------------------------------------CD--------------------------------------
//******************************************************************************
/*
  Le compartiment CD dispose de deux foncions, dont une qui peut eventuellement
  servir sur les autres modules, les fonctions sont les suivantes :
  - lecture_Fichier qui lit un fichier en entree (le nom du fichier a lire est celui renseigne après la balise -i) et qui return le tableau contenant tous les caracteres que ce dernier contenait.
  - XOR qui applique le chiffrage XOR a un tableau de caracteres, et cree ou ecrase un fichier (nom du fichier renseigne apres la balise -o) contenant tous les caracteres du tableau d'entree, mais cryptés.
  Bien entendu la fin de CD se situe apres la creation du fichier cree dans la fonction XOR.
*/

//------------------------------------------------------------------------------

unsigned char* lecture_Fichier(unsigned const char* text_in, int* taille) // les parametres sont : la tableau contenant le nom du fichier in, et un pointeur vers la variable contenant la taille du tableau de caractere qui va contenir le contenu du fichier in.
{
  int taille_alloc = 50; // avec l'allocation de memoire par bloc, j'initialise ma variable taille_alloc a 50, pour que mes blocs soient alloués de 50 en 50.
  FILE* fichier = NULL;
  fichier = fopen((char*)text_in, "r"); // ouverture du fichier
  if (fichier == NULL)
  {
    printf("Erreur Fichier\n");
    exit(-1);
  }

  unsigned char* tab_text_in = malloc(taille_alloc * sizeof(unsigned char)); // allocation du premier bloc de 50 char a mon tableau tab_text_in
  unsigned char temp; // variable temporaire qui va stocker le caractere le temps qu'il soit deplace vers tab_text_in

  while (fscanf(fichier, "%c", &temp) != EOF) // boucle qui parcourt le fichier pour recuperer chaque caractere et le stocker dans tab_text_in
  {
    if ((*taille) == taille_alloc)
    {
      taille_alloc += 50 ; // dans le cas ou le fichier fasse plus de 50 char, on rajoute un bloc de 50 char supplementaire
      tab_text_in = realloc(tab_text_in, sizeof(unsigned char) * (taille_alloc + 2));// on utilise realloc pour rajouter la nouvelle taille necessaire au tableau
    }
    tab_text_in[*taille] = temp; // ajout de temp a tab_text_in
    (*taille)++;
  }
  fclose(fichier); // fermeture du fichier in
  return tab_text_in;
}

//------------------------------------------------------------------------------

void XOR(unsigned char* Text_in_tab, int Text_in_tab_taille, unsigned const char* key, unsigned const char* Text_out) // les parametres sont : le tableau contenant les caracteres du fichier in, la taille de ce dernier, le tableau contenant la cle, le tableau contenant le nom du fichier out.
{
  FILE* Fichier_out = fopen((char*)Text_out, "w"); // creation du fichier de sortie (ou ecrase celui deja existant) et ouverture de ce dernier dans le programme
  int lengh_key = strlen((char*)key); // longueur de la cle
  unsigned char temp;
  int compt_key = 0; // compteur de la clé permettant de remettre a zero les caracteres de la cle lorsque le dernier a ete utilise pour le XOR
  int i = 0;
  while (i < Text_in_tab_taille) // boucle parcourant text_in_tab et qui
  {
    temp = Text_in_tab[i] ^ key[compt_key]; // aplication de l'operation XOR
    fputc(temp, Fichier_out); // ecriture du caractere dans le fichier out
    ++compt_key; // incrementation du compteur servant a reinitialiser la cle
    if (compt_key == lengh_key) // si le caractere de la cle qui a ete utilise est celui de la derniere, alors on prends le premier
    {
      compt_key = 0;
    }
    ++i;
  }
  fclose(Fichier_out); // fermeture du fichier out
}


//******************************************************************************
//-----------------------------------C1-----------------------------------------
//******************************************************************************
/*
  Le compartiment C1 comporte 3 fonctions, permettant en les combinant toutes,
  d'obtenir un tableau de la forme tab[Taille_cle][caracteres possibles],
  permettant de regrouper tous les caracteres possibles pour un caractere inconnu
  de la cle dans un sous tableau qui lui est dedie.
  les fonctions sont les suivantes :
  - Verification_caracteres qui est la fonction principale des trois, elle utilise les deux autres fonctions pour pouvoir traiter les donnees qu'elle recoit, puis rend un tableau contenant les informations demandees.
  - Caractere_valide est une fonction qui permet de savoir si un char fait bien parti du tableau des caracteres autorises par le tableau fourni dans le cahier des charges (pour message). Etant donne la longueur du if, j'ai prefere isole cette fonction au lieu de mettre le if dans la fonction principale.
  - Caractere_cle_valide est une fonction qui va appliquer XOR entre le message et un caractere de cle possible, et verifier si le resultat est bien un caractere autorise pour les messages, et ainsi informer si oui ou non ce caractere peut possiblement etre un caractere de la cle a trouver.
  La fin de C1 se situe a la fin de Verification_caracteres, avec le tableau de forme tab[taille_cle][caracteres_possibles].
*/

//------------------------------------------------------------------------------

int Caractere_valide(unsigned char text) // le parametre est le caractere a tester
{
  /*
  utilise la fonction if, et, a travers tous ces tests finit par
  renvoyer 1 si le caractere est bel et bien un caractere autorise pour le
  latin-1, ou 0 si il n'est pas autorise. (chaque chiffre correspond a un char)
  */
  if((text == 32) || (text >= 65 && text <= 90) || (text >= 97 && text <= 122)
  //Test des ponctuations uniquement messages
  || (text == 33) || (text == 34) || (text == 39) || (text == 40) || (text == 41) || (text == 59)
  //Test des ponctuations cle & messages
  || (text == 44) || (text == 45) || (text == 46) || (text == 58) || (text == 63) || (text == 95) || (text == 123) || (text == 125)
  //Caracteres accentues :
  // Tous les A differents
  || (text == 192) || (text == 194) || (text == 196) || (text == 224) || (text == 226) || (text == 228)
  // Tous les C differents
  || (text == 131) || (text == 199)
  // Tous les E differents
  || (text>=200 && text<=203) || (text>=232 && text<=235)
  // Tous les I differents
  || (text == 206) || (text == 207) || (text == 238) || (text == 239)
  // Tous les U differents
  || (text == 217) || (text == 219) || (text == 249) || (text == 251)
  // Tous les O differents
  || (text == 212) || (text == 214) || (text == 244) || (text == 246)
  // Retour chariot (retour a la ligne)
  || (text == 10))
  {
    return 1; // Si le caractere est valide
  }
  else
  {
    return 0; // Si le caractere est faux
  }
}

//------------------------------------------------------------------------------

int Caractere_cle_valide(unsigned char* text, int taille_text, unsigned char caractere_cle, int indice_cle, int longueur_cle) // les parametres sont : le texte chiffre, la taille du texte, le caractere de cle a tester, l'indice correspondant a sa place dans la cle, la longueur de la cle
{
  for (int i = indice_cle ; i < taille_text ; i += longueur_cle) // boucle qui va tester le caractere actuel pour valeur valeur du texte qu'il est supposé dechiffrer
  {
    if (Caractere_valide(text[i] ^ caractere_cle) == 0) // test du caractere par Caractere_valide
    {
      return 0; // Si le caractere n'est pas valide
    }
  }
  return 1; // Si le caractere est valide
}

//------------------------------------------------------------------------------

unsigned char** Verification_caracteres(unsigned char* text_in_char, int taille_text_in, int taille_cle, int* tab_nb_caracteres) // les parametres sont : le tableau contenant le texte du fichier in, la taille du tableau contenant le fichier in, la taille de la cle, un pointeur vers la variable tab_nb_caracteres
{
  int indice_tableau;
  unsigned char** tab_cle_valide = malloc(taille_cle * sizeof(unsigned char*)); // allocation de la taille du tableau contenant la cle a un tableau qui contiendra les differents caracteres
  unsigned char tab_cle_latin_1[71] = {44, 45, 46, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 63, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 95, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 125};
  // le tableau tab_cle_latin_1 contient tous les codes ascii correspondants aux caracteres valides pour une cle

  for (int i = 0 ; i < taille_cle ; ++i) // booucle qui parcours la longueur de la cle, et va faire tous les tests pour chaque caracteres du tab_cle_latin_1
  {
    indice_tableau = 0; // indice servant a pouvant ajouter dans l'ordre les cles valables (incrémenté quand une cle est ajoutee a tab_cle_valide)
    tab_cle_valide[i] = malloc(70 * sizeof(unsigned char)); // allocation de 70 blocs de la taille char par securite, au cas ou les 70 caracteres seraient valables, afin de ne pas faire un core dumped
    for (int j = 0 ; j < 70 ; ++j) // boucle qui parcour la liste tab_cle_latin_1 pour trouver les caracteres valides
    {
      if (Caractere_cle_valide(text_in_char, taille_text_in, tab_cle_latin_1[j], i, taille_cle) == 1) // test du caractere
      {
        tab_cle_valide[i][indice_tableau++] = tab_cle_latin_1[j]; // le caractere teste s'ajoute a tab_cle_valide si il est correct, le tout en incrementant indice_tableau
      }
    }
    tab_nb_caracteres[i] = indice_tableau; // ajout du nombre de caracteres possibles pour l'indice (indice_tableau) actuel de la cle
  }
  return tab_cle_valide;
}



//******************************************************************************
//---------------------------------------C2-------------------------------------
//******************************************************************************

/*
  Le compartiment C2 contient trois fonctions :
  - Concatenation_main qui est une fonction qui sert a creer toutes les combinaisons de cles possible a partir du tableau de caracteres possibles, elle fait appel à Fonction_for
  - Fonction_for qui est une fonction permettant à Concatenation_main de traiter tab_cle_valide pour n'importe quelle taille de cle, tout simplement en s'auto appelant jusqu'a avoir atteint la profondeur nécessaire
  - Analyse_frequentielle qui est une fonction permettant de trouver quelle clé est la bonne parmis toutes celles proposees par Concatenation_main, en utilisant un systeme de calcul de frequence d'apparition de certaines lettres
  Le module C2 se termine lorsque Analyse_frequentielle a ete utilisee et a retournee la bonne cle pour decrypter.
*/

//La fonction "Fonction_for" definit la fonction qui s'auto appelle pour pouvoir concatener convenablement les clefs en s'adaptant a n'importe quelle taille de cle.
void Fonction_for(long *cles_concatenees_compt, int* tab_nb_carac_cle_valide,  int *rang, int* tab_rangs, int *taille_cle, unsigned char** cles_tab, unsigned char** tab_cle_valide) // les parametres sont : un pointeur vers la variable contenant le nombre de cle crees jusqu'a present, le tableau contenant la longueur de chaque sous-tableau de tab_cle_valide, le rang actuel, le tableau des rangs, la taille de la cle, le tableau des cles, le tableau contenant les caracteres possibles pour la cle
{
  for(int i = 0; i < tab_nb_carac_cle_valide[*rang]; ++i) // boucle qui parcoursle nombre de cles possible pour chaque caractere de la cle
  {
    tab_rangs[*rang] = i; // on reinitialise le rang pour l'utiliser sur un nouveau tableau de caracteres possibles
    if ( *rang < (*taille_cle - 1) ) // s'arrete si on atteint le dernier sous-tableau représentant la derniere liste de caracteres de la cle
    {
      ++(*rang);
      Fonction_for(cles_concatenees_compt, tab_nb_carac_cle_valide, rang, tab_rangs, taille_cle, cles_tab, tab_cle_valide); // la fonction s'auto appelle pour atteindre le sous tableau inferieur
    }
    else
    {
      for(int j = 0; j < *taille_cle; ++j)
      {
        cles_tab[*cles_concatenees_compt][j] = tab_cle_valide[j][tab_rangs[j]]; // on rentre la cle possible dans cles_tab
      }
      ++(*cles_concatenees_compt); // 1 cle de plus cree
    }
  }
  *rang = *rang - 1;
}

//------------------------------------------------------------------------------

unsigned char** Concatenation_main(int* tab_nb_carac_cle_valide, unsigned char** tab_cle_valide, int *taille_cle, long *cles_tab_total) // les parametres sont : le tableau contenant les differentes tailles des sous tableaux de tab_cle_valide, le tableau contenant les caracteres possibles pour chaque caractere de la cle, la taille de la cle et un pointeur vers cles_tab_total qui représentele nombre de combinaisons possible
{
  *cles_tab_total = 1; // initialisation a 1 du total des cles a trouver
  for(int i = 0; i < *taille_cle; ++i) // boucle qui parcours le tableau contenant les sous tableaux
  {
    *cles_tab_total = (*cles_tab_total) * tab_nb_carac_cle_valide[i]; // calcul du nombre de possibilites de cles
  }
  unsigned char** cles_tab = malloc(*cles_tab_total * sizeof(unsigned char*)); // on alloue de la memoire autant qu'il faut pour pouvoir stocker le bon nombre de cles possibles
    for(long i = 0; i < *cles_tab_total; ++i )
    {
      cles_tab[i] = malloc( (*taille_cle) * sizeof(unsigned char)); // on alloue les sous tableaux 1 par 1 avec la taille de la cle pour y rentrer une cle possible
    }
  long cles_concatenees_compt = 0; // initialisation du compteur du nombre de cles crees jusqu'a present
  int rang = 0;
  int* tab_rangs = malloc( (*taille_cle) * sizeof(int)); // allocation du nombre de caracteres de la cle a tab_rangs
  Fonction_for(&cles_concatenees_compt, tab_nb_carac_cle_valide, &rang, tab_rangs, taille_cle, cles_tab, tab_cle_valide); // lancement du processus pseudo "recursif"

  free(tab_rangs);

  return cles_tab;
}


//------------------------------------------------------------------------------

//dans cette fonction j'aurais pu décomposer en plusieurs sous-fonctions, mais j'ai prefere faire comme ça pour avoir tout le processus bien en vue, meme si, ce n'est pas tres clean code, j'obtiens neanmois un temps d'execution vraiment pas mal!
unsigned char* Analyse_frequentielle(unsigned char** cles_tab, unsigned char* Text_in_char, int Taille_texte_in, int Lengh_key) // les parametres sont : le tableau de toutes les cles possibles, le tableau contenant le texte crypté, la taille du texte crypté, et la longueur de la cle
{
  // declaration des variables a utiliser
  unsigned char* cle = malloc(Lengh_key * sizeof(unsigned char));
  unsigned char* Key_final = malloc(Lengh_key * sizeof(unsigned char));
  int nombre_lettres = 0;
  float score = 0.0; // initialisation du score a 0
  float score_final = 100000000000.0; // initialisation du score a battre (tres haut comme ça on est sur que la premiere cle teste est inferieure)
  float tab_freq_th[26] = {9.42, 1.02, 2.64, 3.39, 15.87, 0.95, 1.04, 0.77, 8.41, 0.89, 0.00, 5.34, 3.24, 7.15, 5.14, 2.86, 1.06, 6.46, 7.90, 7.26, 6.24, 2.15, 0.00, 0.30, 0.24, 0.32}; // initialisation du tableau des frequences théoriques
  float tab_occur_lettres[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // initialisation du tableau des occurences (26 * 0)
  int compt_key = 0;
  unsigned char temp;

  // boucle qui va couvrir l'ensemble des clefs
  for (int indice = 0; indice < cles_tab_total; ++indice)
  {
    //reinitialisation des variables temporaires
    compt_key = 0;
    unsigned char* tab_decrypt = malloc(Taille_text_in * sizeof(unsigned char));
    for (int u = 0 ; u < 26 ; ++u)
    {
      tab_occur_lettres[u] = 0;
    }
    score = 0.0; // remise a zero du score, puisque changement de cle
    nombre_lettres = 0; // remise a zero du nombre de lettres du texte

    for (int f = 0; f < Lengh_key; ++f)
    {
      cle[f] = cles_tab[indice][f];
    }

    // decryptage du fichier, sortie sur tab_decrypt
    for (int i = 0; i < Taille_text_in; ++i) // boucle parcourant text_in_char
    {
      temp = Text_in_char[i] ^ cle[compt_key]; // application de l'operation XOR
      tab_decrypt[i] = temp; // ecriture du caractere dans le tableau du texte decrypte
      ++compt_key; // incrementation du compteur servant a reinitialiser la cle
      if (compt_key == Lengh_key) // si le caractere de la cle qui a ete utilise est celui de la derniere, alors on prends le premier
      {
        compt_key = 0;
      }
    }

    // comptage de chaque lettre du fichier decrypte, tableau d'occurrences sur tab_occur_lettres et nombre de lettres sur nombre_lettres
    for (int j = 0; j < Taille_texte_in; ++j)
    {
      temp = tab_decrypt[j];
      // Verification majuscule
      if (temp >= 65 && temp <= 90)
      {tab_occur_lettres[temp - 65] += 1; nombre_lettres += 1;}
      // Verification minuscule
      if (temp >= 97 && temp <= 122)
      {tab_occur_lettres[temp - 97] += 1; nombre_lettres += 1;}
      // Verification A speciaux
      if ((temp == 192) || (temp == 194) || (temp == 196) || (temp == 224) || (temp == 226) || (temp == 228))
      {tab_occur_lettres[0] += 1; nombre_lettres += 1;}
      // Verification C speciaux
      if ((temp == 131) || (temp == 199))
      {tab_occur_lettres[2] += 1; nombre_lettres += 1;}
      // Verification E speciaux
      if ((temp >= 200 && temp <= 203) || (temp >= 232 && temp <= 235))
      {tab_occur_lettres[4] += 1; nombre_lettres += 1;}
      // Verification I speciaux
      if ((temp == 206) || (temp == 207) || (temp == 238) || (temp == 239))
      {tab_occur_lettres[8] += 1; nombre_lettres += 1;}
      // Verification U speciaux
      if ((temp == 217) || (temp == 219) || (temp == 249) || (temp == 251))
      {tab_occur_lettres[20] += 1; nombre_lettres += 1;}
      // Verification O speciaux
      if ((temp == 217) || (temp == 219) || (temp == 249) || (temp == 251))
      {tab_occur_lettres[14] += 1; nombre_lettres += 1;}
    }

    // calcul du nombre de points qu'a la cle testee dans score
    for (int k = 0; k < 26; ++k)
    {
      score += (tab_freq_th[k] - ((tab_occur_lettres[k] / nombre_lettres) * 100)) * (tab_freq_th[k] - ((tab_occur_lettres[k] / nombre_lettres) * 100)) ;
    }
    if (score < score_final) // test pour savoir si la cle actuelle marche mieux que les autres deja essayees
    {
      score_final = score;
      for (int indice_final = 0; indice_final < Lengh_key; ++indice_final)
      {
        Key_final[indice_final] = cle[indice_final]; // remplacement de l'ancienne cle
      }
    }
    free(tab_decrypt);
  }
  free(cle);
  return Key_final;
}


//******************************************************************************
//---------------------------------------C3-------------------------------------
//******************************************************************************
/*
  Le module C3 n'est malheureusement pas acheve, j'ai manque de temps pour le terminer, j'ai neanmois depose la fonction Lecture_dictionnaire que j'ai pu terminer et qui marche, qui lit un dictionnaire et qui renvoit
  les mots <= 4 lettres de longueur dans un tableau a deux dimensions ; en esperant que je ne l'ai pas fait pour rien.
*/

unsigned char** Lecture_dictionnaire(long *nombre_mots_dico, unsigned char* local_dico) // le parametre est le pointeur vers la variable globale representant le nombre de mots qui seront contenus dans la tableau de sortie de la fonction
 {

  // Ouverture du fichier
  FILE* fichier = NULL;
  fichier = fopen((char*)local_dico, "r"); // ouverture du fichier

  // Verification de l'existance du dictionnaire
  if (fichier == NULL)
  {
    printf("Erreur Dictionnaire\n");
    exit(-1);
  }

  // Allocations initiales & initialisation de variables
  int longueur_autorisee = 5; // Nombre de lettres maxi dans un mot + 1
  unsigned char** tab_text_in = malloc(1 * sizeof(unsigned char*)); // allocation de memoire pour le tableau de sortie contenant toutes les strings
  tab_text_in[0] = malloc((longueur_autorisee + 1) * sizeof(unsigned char));// allocation de memoire pour les sous tableaux contenant des strings de maximum 5
  int longueur_mot_actuel = 0; // initialisation du compteur pour la taille du mot en train d'etre extrait du dico
  unsigned char temp; // Variable temporaire qui va stocker le caractere le temps qu'il soit deplace vers le tableau du dictionnaire
  *nombre_mots_dico = 0;
  unsigned char tab_stock_mot[27]; // initialisation du tableau permettant de stocker les mots (la taille est celle du plus long mot de l'alphabet + 1)

  while ( fscanf(fichier, "%c", &temp) != EOF) // boucle qui va parcourir tout le fichier in
  {
    if ( temp != 10 ) // Si jamais le caractere est un retour a la ligne ( 10 = code ascii du retour chariot) on va changer de mot
    {
      if ( longueur_mot_actuel < (longueur_autorisee + 1) ) // si jamais le mot depasse la limite de 4 lettres
      {
        tab_stock_mot[longueur_mot_actuel] = temp; // on rajoute temp au tableau de stockage temporaire a la position qui correspond a la position dans le mot
      }
    ++longueur_mot_actuel; // la longueur du mot actuel augmente de 1
    }
    else
    {
      if (longueur_mot_actuel < (longueur_autorisee + 1)) // si le mot teste precedemment est superieur a 4 lettres, alors on ne le garde pas
      {
        tab_text_in = realloc(tab_text_in, (*nombre_mots_dico + 2) * sizeof(unsigned char*)); // allocation de memoire pour le tableau contenant tous les strings de mots de 4 lettres maxi
        tab_text_in[*nombre_mots_dico + 1] = malloc((longueur_autorisee + 1) * sizeof(unsigned char)); // allocation de memoire pour la string (mot) actuel
        for ( int i = 0; i < longueur_mot_actuel; ++i) // boucle qui parcourt le tableau temporaire qui a stocke le mot en cours de traitement dans le tableau de sortie
        {
          tab_text_in[*nombre_mots_dico][i] = tab_stock_mot[i];
        }
        tab_text_in[*nombre_mots_dico][longueur_mot_actuel] = '\0'; // on place le caractere de fin de string
        ++(*nombre_mots_dico); // On augmente le nombre de mots stockes dans le tableau de sortie de 1
      }
      longueur_mot_actuel = 0; // reinitialisation de la longueur du mot actuel pour pouvoir traiter le mot suivant
    }
  }

  if (longueur_mot_actuel < (longueur_autorisee + 1)) // on refait la condition en dehors pour le dernier mot car il n'y a pas de retour chariot
  {
    tab_text_in = realloc(tab_text_in, (*nombre_mots_dico + 2) * sizeof(unsigned char*));
    tab_text_in[*nombre_mots_dico + 1] = malloc((longueur_autorisee + 1) * sizeof(unsigned char));
    for ( int i = 0; i < longueur_mot_actuel; ++i)
    {
      tab_text_in[*nombre_mots_dico][i] = tab_stock_mot[i];
    }
    tab_text_in[*nombre_mots_dico][longueur_mot_actuel] = '\0';
    ++(*nombre_mots_dico);
  }
  return tab_text_in; // retour du tableau contenant le dictionnaire avec maxi 4 lettres par la fonction
}

//******************************************************************************
//                         Fonctions supplementaires
//******************************************************************************

int cle_valide(char text) // le parametre est le caractere a tester
{
  /*
  utilise la fonction if, et, a travers tous ces tests finit par
  renvoyer 1 si le caractere est bel et bien un caractere autorise pour la
  cle, ou 0 si il n'est pas autorise. (chaque chiffre correspond a un char)
  */
  // Tests nombres et lettres(MAJ et min)
  if((text >= 65 && text <= 90) || (text >= 97 && text <= 122) || (text >= 48 && text <= 57)
  //Test des ponctuations cle
  || (text == 44) || (text == 45) || (text == 46) || (text == 58) || (text == 63) || (text == 95) || (text == 123) || (text == 125))
  {
    return 1; // Si le caractere est valide
  }
  else
  {
    return 0; // Si le caractere est faux
  }
}

//------------------------------------------------------------------------------

void Test_cle(char* key) // fonction pour tester si les caracteres de la cle sont valides d'apres le tableau des caracteres autorises
{
  int Taille_cle = strlen(key);
  for (int i = 0 ; i < Taille_cle ; ++i)
  {
    if (cle_valide(key[i]) == 0)
    {
      printf("Erreur Clé\n");
      exit(-1);
    }
  }
}

//------------------------------------------------------------------------------

void Temps_execution(clock_t start)// fonction pour calculer le temps d'execution de la tache que l'on vient de demander au programme
{
  clock_t end = clock(); // declaration de end au "clock" actuel
  fprintf(stderr , "Effectué en %.8f secondes.\n",(double)(end - start) / CLOCKS_PER_SEC);
}



//******************************************************************************
//----------------------------------------Main----------------------------------
//******************************************************************************


int main(int argc, char **argv)
{
  clock_t start = clock(); // debut du compteur
  // Lecture des arguments et erreurs
  if ((argc % 2) == 0)
  {
    printf("Erreur Commande\n");
    exit(-1);
  }
  for(int i = 0; i < argc ; ++i)
  {
    char* temp = argv[i];

    if ((strncmp(temp, "-i", 2 ) == 0) && ((i % 2) != 0))
      {

        Text_in = (unsigned char*)argv[i+1];
        Text_in_compt = 1;
      }
      // récupère le chemin du fichier à ouvrir, et le stocke dans la variable Text_in
    if (strncmp(temp, "-o", 2 ) == 0)
      {
        Text_out = (unsigned char*)argv[i+1];
        Text_out_compt = 1;
      }
      // récupère le chemin du fichier qui sera créé par la suite, et où le texte encrypté sera stocké, et le stocke dans la variable Text_out
    if ( strncmp(temp, "-k", 2 ) == 0)
      {
        Test_cle(argv[i+1]);
        Key = (unsigned char*)argv[i+1];
        Key_compt = 1;
      }
      // récupère la clé de cryptage, et la stocke dans la variable Key
    if ( strncmp(temp, "-l", 2 ) == 0)
      {
        Lengh_key = (int)strtol(argv[i+1], NULL, 10);
        Lengh_key_compt = 1;
      }
      // récupère la longueur de la cle de cryptage, et la stocke dans la variable lengh_key
    if ( strncmp(temp, "-m", 2 ) == 0)
      {
        Mode = (char*)argv[i+1];
        Mode_compt = 1;
      }
      //récupère le mode utilise par l'utilisateur et le stocke dans la variable Mode
  }
  total_args = Mode_compt + Lengh_key_compt + Key_compt + Text_out_compt + Text_in_compt;
  total_args = (total_args * 2) + 1;

  if (total_args != argc)
  {
    printf("Erreur Commande\n");
    exit(-1);
  }
  //----------------------------------------------------------------------------
  //                                    CD
  //----------------------------------------------------------------------------
  if (Text_in_compt == 1 && Text_out_compt == 1 && Key_compt == 1 && Lengh_key_compt == 0 && Mode_compt == 0)
  {
    unsigned char* Text_in_char = lecture_Fichier(Text_in, &Taille_text_in);
    XOR(Text_in_char, Taille_text_in, Key, Text_out);

    //--------Liberation des malloc-------------
    free(Text_in_char);

    return 0;
  }
  //----------------------------------------------------------------------------
  //                                    C1
  //----------------------------------------------------------------------------
  if ((Text_in_compt == 1 && Text_out_compt == 0 && Key_compt == 0 && Lengh_key_compt == 1 && Mode_compt == 1) && strncmp(Mode, "1", 1 ) == 0)
  {
    int tab_nb_carac_cle_valide[Lengh_key];
    unsigned char* Text_in_char = lecture_Fichier(Text_in, &Taille_text_in);
    unsigned char** tab_cle_valide = Verification_caracteres(Text_in_char, Taille_text_in, Lengh_key, tab_nb_carac_cle_valide);
    for(int i = 0 ; i < Lengh_key ; ++i)
    {
      printf("[");
      for(int j = 0 ; j < tab_nb_carac_cle_valide[i] ; ++j)
      {
        printf("%c" , tab_cle_valide[i][j]);
      }
      printf("]");
    }
    printf("\n");

    //--------Liberation des malloc-------------
    for (int i = 0 ; i < Lengh_key ; ++i)
    {
      free(tab_cle_valide[i]);
    }
    free(Text_in_char);
    free(tab_cle_valide);

    Temps_execution(start);
    return 0;
  }
  //----------------------------------------------------------------------------
  //                                    C2
  //                    Avec & Sans longueur de cle fournie
  //----------------------------------------------------------------------------

//-------------------------Avec la longueur de la clef--------------------------

  if ((Text_in_compt == 1 && Text_out_compt == 0 && Key_compt == 0 && Lengh_key_compt == 1 && Mode_compt == 1) && strncmp(Mode, "2", 1 ) == 0)
  {
    int tab_nb_carac_cle_valide[Lengh_key];
    unsigned char* Text_in_char = lecture_Fichier(Text_in, &Taille_text_in);
    unsigned char** tab_cle_valide = Verification_caracteres(Text_in_char, Taille_text_in, Lengh_key, tab_nb_carac_cle_valide);
    unsigned char** cles_tab = Concatenation_main(tab_nb_carac_cle_valide, tab_cle_valide, &Lengh_key, &cles_tab_total);
    unsigned char* cle_finale = Analyse_frequentielle(cles_tab, Text_in_char, Taille_text_in, Lengh_key);
    for(int i = 0 ; i < Lengh_key ; ++i)
    {
        printf("%c" , cle_finale[i]);
    }
    printf("\n");

    //--------Liberation des malloc-------------
    for (int i = 0 ; i < Lengh_key ; ++i)
    {
      free(tab_cle_valide[i]);
    }
    for (int i = 0 ; i < cles_tab_total ; ++i)
    {
      free(cles_tab[i]);
    }
    free(Text_in_char);
    free(tab_cle_valide);
    free(cles_tab);
    free(cle_finale);

    Temps_execution(start);
    return 0;
  }

//-------------------------Sans la longueur de la clef--------------------------

  if ((Text_in_compt == 1 && Text_out_compt == 0 && Key_compt == 0 && Lengh_key_compt == 0 && Mode_compt == 1) && strncmp(Mode, "2", 1 ) == 0)
  {
    unsigned char* Text_in_char = lecture_Fichier(Text_in, &Taille_text_in);

    //-----------------------------Longueur 3-----------------------------------

    Lengh_key = 3;
    int tab_nb_carac_cle_valide_3[Lengh_key];
    unsigned char** tab_cle_valide = Verification_caracteres(Text_in_char, Taille_text_in, Lengh_key, tab_nb_carac_cle_valide_3);
    unsigned char** cles_tab = Concatenation_main(tab_nb_carac_cle_valide_3, tab_cle_valide, &Lengh_key, &cles_tab_total);
    unsigned char* cle_finale = Analyse_frequentielle(cles_tab, Text_in_char, Taille_text_in, Lengh_key);
    for(int i = 0 ; i < Lengh_key ; ++i)
    {
      printf("%c" , cle_finale[i]);
    }
    printf("\n");

    //--------Liberation des malloc-------------
    for (int i = 0 ; i < Lengh_key ; ++i)
    {
      free(tab_cle_valide[i]);
    }
    for (int i = 0 ; i < cles_tab_total ; ++i)
    {
      free(cles_tab[i]);
    }
    free(tab_cle_valide);
    free(cles_tab);
    free(cle_finale);

    //-----------------------------Longueur 4-----------------------------------

    Lengh_key = 4;
    cles_tab_total = 0;
    int tab_nb_carac_cle_valide_4[Lengh_key];
    tab_cle_valide = Verification_caracteres(Text_in_char, Taille_text_in, Lengh_key, tab_nb_carac_cle_valide_4);
    cles_tab = Concatenation_main(tab_nb_carac_cle_valide_4, tab_cle_valide, &Lengh_key, &cles_tab_total);
    cle_finale = Analyse_frequentielle(cles_tab, Text_in_char, Taille_text_in, Lengh_key);
    for(int i = 0 ; i < Lengh_key ; ++i)
    {
      printf("%c" , cle_finale[i]);
    }
    printf("\n");

    //--------Liberation des malloc-------------
    for (int i = 0 ; i < Lengh_key ; ++i)
    {
      free(tab_cle_valide[i]);
    }
    for (int i = 0 ; i < cles_tab_total ; ++i)
    {
      free(cles_tab[i]);
    }
    free(tab_cle_valide);
    free(cles_tab);
    free(cle_finale);

    //-----------------------------Longueur 5-----------------------------------

    Lengh_key = 5;
    cles_tab_total = 0;
    int tab_nb_carac_cle_valide_5[Lengh_key];
    tab_cle_valide = Verification_caracteres(Text_in_char, Taille_text_in, Lengh_key, tab_nb_carac_cle_valide_5);
    cles_tab = Concatenation_main(tab_nb_carac_cle_valide_5, tab_cle_valide, &Lengh_key, &cles_tab_total);
    cle_finale = Analyse_frequentielle(cles_tab, Text_in_char, Taille_text_in, Lengh_key);
    for(int i = 0 ; i < Lengh_key ; ++i)
    {
      printf("%c" , cle_finale[i]);
    }
    printf("\n");

    //--------Liberation des malloc-------------
    for (int i = 0 ; i < Lengh_key ; ++i)
    {
      free(tab_cle_valide[i]);
    }
    for (int i = 0 ; i < cles_tab_total ; ++i)
    {
      free(cles_tab[i]);
    }
    free(tab_cle_valide);
    free(cles_tab);
    free(cle_finale);

    //-----------------------------Longueur 6-----------------------------------

    Lengh_key = 6;
    cles_tab_total = 0;
    int tab_nb_carac_cle_valide_6[Lengh_key];
    tab_cle_valide = Verification_caracteres(Text_in_char, Taille_text_in, Lengh_key, tab_nb_carac_cle_valide_6);
    cles_tab = Concatenation_main(tab_nb_carac_cle_valide_6, tab_cle_valide, &Lengh_key, &cles_tab_total);
    cle_finale = Analyse_frequentielle(cles_tab, Text_in_char, Taille_text_in, Lengh_key);
    for(int i = 0 ; i < Lengh_key ; ++i)
    {
      printf("%c" , cle_finale[i]);
    }
    printf("\n");

    //--------Liberation des malloc-------------
    for (int i = 0 ; i < Lengh_key ; ++i)
    {
      free(tab_cle_valide[i]);
    }
    for (int i = 0 ; i < cles_tab_total ; ++i)
    {
      free(cles_tab[i]);
    }
    free(tab_cle_valide);
    free(cles_tab);
    free(cle_finale);

    //-----------------------------Longueur 7-----------------------------------

    Lengh_key = 7;
    cles_tab_total = 0;
    int tab_nb_carac_cle_valide_7[Lengh_key];
    tab_cle_valide = Verification_caracteres(Text_in_char, Taille_text_in, Lengh_key, tab_nb_carac_cle_valide_7);
    cles_tab = Concatenation_main(tab_nb_carac_cle_valide_7, tab_cle_valide, &Lengh_key, &cles_tab_total);
    cle_finale = Analyse_frequentielle(cles_tab, Text_in_char, Taille_text_in, Lengh_key);
    for(int i = 0 ; i < Lengh_key ; ++i)
    {
      printf("%c" , cle_finale[i]);
    }
    printf("\n");


    //--------Liberation des malloc-------------
    for (int i = 0 ; i < Lengh_key ; ++i)
    {
      free(tab_cle_valide[i]);
    }
    for (int i = 0 ; i < cles_tab_total ; ++i)
    {
      free(cles_tab[i]);
    }
    free(tab_cle_valide);
    free(cles_tab);
    free(cle_finale);
    free(Text_in_char);

    Temps_execution(start);
    return 0;
  }


  printf("Erreur Commande\n");
  exit(-1);
  return 0;
}
