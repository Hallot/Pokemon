#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Pour strcmp
#include <time.h> //Pour time dans la fonction random


//Défini une structure Attaque
typedef struct Attack
{
  char* Nom;
  int Force;
  int Type;
}Attaque;


//Fonction qui transforme un type définie avec une chaine en son entier correspondant
int string_type_to_int (char* type)
{
  if (strncmp(type, "Feu", 5) == 0) return(1);
  if (strncmp(type, "Eau", 5) == 0) return(2);
  if (strncmp(type, "Foudre", 10) == 0) return(3);
  if (strncmp(type, "Herbe", 10) == 0) return(4);
  // Renvoi un message d'erreur si le type est invalide
  fprintf(stderr, "Le type de l'attaque doit etre \"Feu\", \"Eau\", \"Foudre\" ou \"Herbe\", %s est invalide", type);
  exit(-1);
}


//Inverse de la fonction string_type_to_int, prend un entier et renvoi le type de l'attaque en string
char* int_type_to_string (int type)
{
  if (type == 1) return("Feu");
  if (type == 2) return("Eau");
  if (type == 3) return("Foudre");
  if (type == 4) return("Herbe");
  // Renvoi un message d'erreur si le type est invalide
  fprintf(stderr, "Le type de l'attaque doit etre 1, 2, 3 ou 4, %d est invalide", type);
  exit(-1);
}


//Fonction clear qui nettoie le shell
void nettoie (void)
{
  //Appelle la commande shell "clear"
  system("clear");
}


//Fonction qui crée une attaque
Attaque new_attack (char* nom, int force, char* type)
{
  Attaque a = {nom, force, string_type_to_int(type)};
  return(a);
}


//Affiche une attaque sur le shell
void get_attack (Attaque att)
{
  printf("\t(%s, %s, %d)\n", att.Nom, int_type_to_string(att.Type), att.Force);
}


//Défini une structure pokemon
typedef struct Pkmn
{
  char* Nom;
  int Type;
  int LVL;
  int ATK;
  int DEF;
  int HPMAX;
  int HP;
  Attaque Attaques[4];
}Pokemon;


//Fonction qui crée un pokemon
Pokemon new_pokemon (char* nom, char* type, int lvl, int atk, int def, int hpmax, int hp, Attaque att1, Attaque att2, Attaque att3, Attaque att4)
{
  Pokemon pok = {nom, string_type_to_int(type), lvl, atk, def, hpmax, hp};
  pok.Attaques[0] = att1;
  pok.Attaques[1] = att2;
  pok.Attaques[2] = att3;
  pok.Attaques[3] = att4;
  return(pok);
}


//Fonction qui affiche sur le shell la vie restante sous forme de barre
void get_life (Pokemon pok)
{
  int frac = (10 * pok.HP/pok.HPMAX);
  int i;
  printf("[");
  for (i = 0; i < frac; i++) printf("=");
  //Rmmpli le reste d'espaces jusqu'à ce que la somme des deux fasse 10
  for (i = 0; i < (10 - frac); i++) printf(" ");
  printf("]");
}


//Fonction qui propose 2 affichages des caractéristiques du pokemon en fonction de l'entier : 0 sans attaques, 1 avec. Les \t et \n sont pour la présentation
void get_Pokemon (int choix, Pokemon pok)
{
  if (choix == 0) //sans les attaques
    {
      printf("Adversaire\tPokemon:%s  \tNiveau:%d\tType:%s\n\t\t", pok.Nom, pok.LVL, int_type_to_string(pok.Type));
      get_life(pok);
      printf("\t\tVie:%d/%d\n\n\n\n", pok.HP, pok.HPMAX);
    }
  else if (choix == 1) //avec les attaques
    {
      printf("Joueur\t\tPokemon:%s  \tNiveau:%d\tType:%s\n\t\t", pok.Nom, pok.LVL, int_type_to_string(pok.Type));
      get_life(pok);
      printf("\t\tVie:%d/%d\n\t\tAttaque:%d\t\tDefense:%d\n", pok.HP, pok.HPMAX, pok.ATK, pok.DEF);
      printf("Attaques : \n");
      get_attack(pok.Attaques[0]);
      get_attack(pok.Attaques[1]);
      get_attack(pok.Attaques[2]);
      get_attack(pok.Attaques[3]);
      printf("\n\n\n");
    }
  else 
    {
      fprintf(stderr, "Il n'y a que 2 modes d'affichage : 0 et 1, %d est invalide", choix);
      exit(-1);
    }
}


//Augmente d'un niveau un pokemon
Pokemon level_up (Pokemon pok)
{
  pok.ATK = pok.ATK + pok.LVL;
  pok.DEF = pok.DEF + pok.LVL;
  //J'ai rajouté une augmentation des points de vie max pour une question de gameplay
  pok.HPMAX = pok.HPMAX + pok.LVL;
  (pok.LVL)++;
  return(pok);
}


//Fonction effet qui calcule le multiplicateur (x2 pour rester en entier) associé aux types
int effect (Pokemon pok, Attaque att)
{
  int effet[4][4];
  int i;
  int j;
  for (i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++)
	{
	  //Rempli le tableau avec les valeurs en utilisant le fait que les valeurs sont égales sur chaque diagonale
	  if ((i % 4) == (j % 4) || (i % 4) == ((j + 2) % 4)) effet[i][j] = 2;
	  if ((i % 4) == ((j + 1) % 4)) effet[i][j] = 1;
	  if ((i % 4) == ((j + 3) % 4)) effet[i][j] = 4;
	}
    }
  return(effet[pok.Type - 1][att.Type - 1]);
}


//Fonction qui calcule les dommages d'une attaque du pokemon 1 sur le pokemon 2, et l'affiche
int hit (Attaque att, Pokemon pok1, Pokemon pok2)
{
  //Divise par 2 pour contrer le fois 2 de effect
  int N = effect(pok2, att) * (pok1.ATK + att.Force) / (2 * pok2.DEF);
  printf("%s a utilise %s.\n", pok1.Nom, att.Nom);
  //Affiche l'efficacité de l'attaque
  if (effect(pok2, att) == 4)
    {
      printf("C'est tres efficace !\n");
    }
  else if (effect(pok2, att) == 1)
    {
      printf("Ce n'est pas tres efficace.\n");
    }
  printf("%s a pris %d points de dégats.\n\n", pok2.Nom, N);
  return(N);
}


//Fonction qui demande l'attaque à effectuer au joueur, et renvoie l'attaque
Attaque choix_attaque (Pokemon pok)
{
  int attselect;
  char nombre[1];
  printf("Selectionner une attaque entre 1 et 4.\n");
  scanf("%s", nombre);
  attselect = atoi(nombre);
  //Teste si l'input est bien entre 1 et 4
  if (attselect < 1 || attselect > 4)
    {
      printf("Veuillez selectionner une attaque entre 1 et 4, votre entree est invalide.\n\n");
      return(choix_attaque(pok));
    }
  return(pok.Attaques[attselect - 1]);
 }


//Fonction qui teste si un pokemon est en vie ou non
int en_vie (Pokemon pok)
{
  if (pok.HP <= 0) return(0);
  return(1);
}


//Fonction max intuitive entre 2 entiers
int max (int a, int b)
{
  int res = a;
  if (b > res) res = b;
  return(res);
}


//Fonction min intuitive entre 2 entiers
int min (int a, int b)
{
  int res = a;
  if (b < res) res = b;
  return(res);
}


//Fonction attaque simple de l'AI, choisie aléatoirement une attaque en 0 et 3
Attaque att_AI_rand (Pokemon pok)
{
  int att;
  srand( (unsigned)time( NULL ) ); 
  att = (random() % 4);
  return(pok.Attaques[att]);
}

//Fonction attaque de l'AI qui cherche le maximum de dommage
Attaque att_AI_max (Pokemon pok1, Pokemon pok2)
{
  int i;
  int res = effect(pok2, pok1.Attaques[0]) * (pok1.ATK + pok1.Attaques[0].Force) / (2 * pok2.DEF);
  int j = 0;
  int hit = 0;
  //
  for (i = 1; i < 4; i++)
    {
      hit = effect(pok2, pok1.Attaques[i]) * (pok1.ATK + pok1.Attaques[i].Force) / (2 * pok2.DEF);
      if (max(res, hit) != res) 
	{
	  res = hit;
	  j = i;
	}
    }
  //Renvoie l'attaque la plus forte
  return(pok1.Attaques[j]);
}

//Fonction attaque de l'AI qui cherche le minimum de dommage
Attaque att_AI_min (Pokemon pok1, Pokemon pok2)
{
  int i;
  int res = effect(pok2, pok1.Attaques[0]) * (pok1.ATK + pok1.Attaques[0].Force) / (2 * pok2.DEF);
  int j = 0;
  int hit = 0;
  //
  for (i = 1; i < 4; i++)
    {
      hit = effect(pok2, pok1.Attaques[i]) * (pok1.ATK + pok1.Attaques[i].Force) / (2 * pok2.DEF);
      if (min(res, hit) != res) 
	{
	  res = hit;
	  j = i;
	}
    }
  //Renvoie l'attaque la plus faible
  return(pok1.Attaques[j]);
}


//Fonction qui fait le combat entre 2 pokemons, renvoie si le joueur à gagner (1) ou non (0)
int combat (Pokemon pok1, Pokemon pok2, int diff)
{
  Attaque attselect;
  Attaque attAI;
  int dommage;
  //Affiche les deux pokemons selon l'affichage qui leur correspond (1 pour le joueur, 0 l'adversaire)
  get_Pokemon(0, pok2);
  get_Pokemon(1, pok1);
  //Demande au joueur de choisir une attaque
  attselect = choix_attaque(pok1);
  //Netoie l'écran
  nettoie();
  //Calcule et inflige les dommages au pokemon adverse
  dommage = hit(attselect, pok1, pok2);
  pok2.HP = pok2.HP - dommage;
  //Si le pokemon adverse est en vie, sinon nettoie l'écran et fini le combat
  if (en_vie(pok2))
    {
      //Choisi une attaque selon l'AI et inflige les dommages
      if (diff == 1) attAI = att_AI_min(pok2, pok1);
      if (diff == 2) attAI = att_AI_rand(pok2);
      if (diff == 3) attAI = att_AI_max(pok2, pok1);
      dommage = hit(attAI, pok2, pok1);
      pok1.HP = pok1.HP - dommage;
      //Si le pokemon du joueur est en vie, on recommence toute la séquence, sinon on affiche le message de défaite
      if (en_vie(pok1)) return(combat(pok1, pok2, diff));
      nettoie();
      printf("Dommage ! Vous avez perdu le combat contre %s.\n\n", pok2.Nom);
      return(0);
    }
  nettoie();
  printf("Bravo ! Vous avez gagne le combat contre %s.\n\n", pok2.Nom);
  return(1);
}


//Permet de choisir le mode de jeu
int difficulte (void)
{
  char nombre[1];
  int diff_select;
  //Affiche les difficultés possibles
  printf("Veuillez choisir votre difficulte.\n\n");
  printf("1 Difficulte facile.\n");
  printf("2 Difficulte moyenne.\n");
  printf("3 Difficulte impossible.\n\n");
  printf("Entrez un nombre entre 1 et 3 pour choisir votre difficulte.\n\n\n");
  scanf("%s", nombre);
  diff_select = atoi(nombre);
  //Teste si l'input est bien entre 1 et 5
  if (diff_select < 1 || diff_select > 5)
    {
      nettoie();
      printf("Veuillez selectionner une difficulte entre 1 et 3, votre entree est invalide.\n\n");
      return(difficulte());
    }
  if (diff_select == 2 || diff_select == 3) 
    {
      nettoie();
      printf("Ceci est trop difficile pour votre niveau, veuillez choisir l'option facile.\n\n");
      return(difficulte());
    }
  if (diff_select == 1) return(1);
  if (diff_select == 4) return(2);
  if (diff_select == 5) return(3);
  return(difficulte());
}



//Permet au joueur de choisir un pokemon au début du jeu
Pokemon choisit_debut (Pokemon pok1, Pokemon pok2, Pokemon pok3, Pokemon pok4)
{
  char nombre[1];
  int pkmn_select;
  //Affiche les 4 pokemons et leurs attaques
  get_Pokemon(1, pok1);
  get_Pokemon(1, pok2);
  get_Pokemon(1, pok3);
  get_Pokemon(1, pok4);
  printf("Veuillez choisir votre pokemon en entrant un nombre entre 1 et 4.\n\n");
  scanf("%s", nombre);
  pkmn_select = atoi(nombre);
  //Teste si l'input est bien entre 1 et 4
  if (pkmn_select < 1 || pkmn_select > 4)
    {
      printf("Veuillez selectionner un pokemon entre 1 et 4, votre entree est invalide.\n\n");
      return(choisit_debut (pok1, pok2, pok3, pok4));
    }
  if (pkmn_select == 1)  return(pok1);
  if (pkmn_select == 2)  return(pok2);
  if (pkmn_select == 3)  return(pok3);
  if (pkmn_select == 4)  return(pok4);
  //N'est pas censé servir, mais au cas où relance la fonction
  return(choisit_debut (pok1, pok2, pok3, pok4));
 }


//Définition d'une liste d'adversaire (de type pokemon)
typedef struct maillon *Liste;
struct maillon
{
  Pokemon adv;
  Liste suivant;
};


//Ajout d'un élément en tête de liste
Liste ajoute (Pokemon pok, Liste l)
{
  Liste p = malloc(sizeof(struct maillon));
  p -> adv = pok;  
  p -> suivant = l;
  return(p);
}


//Main fonction
int main(void)
{
  int fini = 1;
  int nb_victoire = 0;
  Liste Adversaires = NULL;
  Pokemon mon_pok;
  int diff = 1;

  //Défini les attaques possibles
  Attaque PO = new_attack("Poseidon", 90, "Eau");
  Attaque CH = new_attack("Chalchiuhtlicue", 70, "Eau");
  Attaque LL = new_attack("Llyr", 50, "Eau");
  Attaque AO = new_attack("Ao Kuang", 30, "Eau");
  Attaque HE = new_attack("Hephaestus", 90, "Feu");
  Attaque SV = new_attack("Svarog", 70, "Feu");
  Attaque AR = new_attack("Arshi Tngri", 50, "Feu");
  Attaque TO = new_attack("Tohil", 30, "Feu");
  Attaque ZE = new_attack("Zeus", 90, "Foudre");
  Attaque TH = new_attack("Thunraz", 70, "Foudre");
  Attaque TA = new_attack("Taranis", 50, "Foudre");
  Attaque AT = new_attack("Atamshkai", 30, "Foudre");
  Attaque AN = new_attack("Antheia", 90, "Herbe");
  Attaque KE = new_attack("Kernunnos", 70, "Herbe");
  Attaque YU = new_attack("Yum Caax", 50, "Herbe");
  Attaque EM = new_attack("Emesh", 30, "Herbe");

  //Défini les pokemons possibles
  Pokemon Meliae = new_pokemon("Meliae", "Herbe", 5, 15, 25, 25, 25, KE, TA, LL, TO);
  Pokemon Callirhoe = new_pokemon("Callirhoe", "Eau", 5, 20, 20, 25, 25, CH, AR, YU, AT);
  Pokemon Melinoe = new_pokemon("Melinoe", "Feu", 5, 25, 15, 25, 25, SV, TA, YU, AO);
  Pokemon Eratheis = new_pokemon("Eratheis", "Foudre", 5, 15, 15, 40, 40, TH, LL, AR, EM);
  Pokemon Kimeira = new_pokemon("Kimeira", "Feu", 15, 50, 50, 100, 100, PO, HE, ZE, AN);
  
  //Nettoie l'écran au début du jeu
  nettoie();

  //Le joueur choisit la difficulté
  diff = difficulte();
  
  //Le joueur choisit son pokemon
  mon_pok = choisit_debut (Meliae, Callirhoe, Melinoe, Eratheis);

  //Création de la liste des pokemons adverses Kimeira en dernier pour les modes autres que facile
  if (diff != 1) Adversaires = ajoute(Kimeira, Adversaires);
  Adversaires = ajoute(Meliae, Adversaires);
  Adversaires = ajoute(Callirhoe, Adversaires);
  Adversaires = ajoute(Melinoe, Adversaires);
  Adversaires = ajoute(Eratheis, Adversaires);

  //Nettoie l'écran après avoir choisit le pokemon
  nettoie();

  //Fait des combats tant que le joueur n'a pas perdu, ou qu'il reste des pokemons à combattre
  while (fini && (Adversaires != NULL))
    {
      printf("Votre pokemon %s va affronter %s. Bonne chance !\n\n\n\n", mon_pok.Nom, (Adversaires -> adv).Nom);
      //Si le joueur à gagner
      if ((combat (mon_pok, Adversaires -> adv, diff)) == 1)
	{
	  //Monte le niveau du pokemon
	  mon_pok = level_up(mon_pok);
	  printf("Bravo ! Vous avez gagne un niveau, %s est maintenant niveau %d.\n\n\n\n", mon_pok.Nom, mon_pok.LVL);
	  //Le soigne
	  mon_pok.HP = mon_pok.HPMAX;
	  //Calcule le nombre de combats remportés
	  nb_victoire++;
	  //Si c'était le dernier combat, gagné
	  if (Adversaires -> suivant == NULL) 
	    {
	      printf("Vous avez defait tous vos adversaires.\nGagne !\n\n");
	      fini = 0;
	    }
	  //Sinon on passe au pokemon suivant
	  else
	    {
	      Adversaires = Adversaires -> suivant;
	    }
	}
      //Si le joueur à perdu, on fini le jeu
      else
	{
	  fini = 0;
	  //Avec ou sans s à victoire selon (0,1) ou plus
	  if (nb_victoire > 1)
	    {
	      printf("Dommage, vous avez perdu apres %d victoires.\n\n", nb_victoire);
	    }
	  else
	    {
	      printf("Dommage, vous avez perdu apres %d victoire.\n\n", nb_victoire);
	    }
	}
    }
  return(0);
}
