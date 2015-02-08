/*
* \file main.cpp
* \brief compte le nombre de fille a une evenement facebook
* \author 
* \version 1.0
* \date 12/12/2014
*/

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <algorithm>
#include <winsock2.h>
#include <math.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

static const string FichierGars = "listegars.txt";
static const string FicherFille = "listefilles.txt";
static const string API1 = "api.genderize.io";
//static const string API2 = "gender-api.com";
vector<string> LectureFichier(string, vector<string>);
vector<string> LecturePrenom(string, vector<string>,string);
void VerificationNonTrouver(vector<string>*, vector<string>*, vector<string>*,string);
string enleverAccents(string);  // Néccéssaire d'enlever les accents avant la requête à l'API pour ne pas avoir des erreurs
string AccentsEnMajuscule(string);  // Aide à trouver plus de match dans le dictionnaire en mettant les caractères spéciaux des noms en majuscule
void AjoutNomFichier(string, string);

int main()
{
	string line;
	vector<string> Participants;  // fichier source provenant de Facebook
	vector<string> PrenomsFilles; //dictionnaire
	vector<string> PrenomsGars;  // dictionnaire
	vector<string> FillesParticipantes; // personne qui a été classé en tant que fille
	vector<string> GarsParticipants; // personne qui a été classé en tant que gars

	vector<string> * Monpointeur1;
	Monpointeur1 = &Participants;

	vector<string> * Monpointeur2;
	Monpointeur2 = &FillesParticipantes;

	vector<string> * Monpointeur3;
	Monpointeur3 = &GarsParticipants;

	Participants = LectureFichier(line, Participants);  // lire le fichier source Facebook
	PrenomsFilles = LecturePrenom(line, PrenomsFilles, FicherFille);  // Lire le dictionnaire de fille
	PrenomsGars = LecturePrenom(line, PrenomsGars, FichierGars); // Lire le dictionnaire de gars

	int flag = 0;
	int flag2 =0;
	
	cout << "Nombre de participants : " << Participants.size() << endl;

	// Recherche dans le dictionnaire des prénoms masculins
	recherchedictionnaire:
	for (unsigned int i = 0; i < Participants.size(); i++)
	{
		
		for (unsigned int e = 0; e < PrenomsGars.size(); e++)
		{
			if (Participants[i].compare(PrenomsGars[e]) == 0)
			{
				GarsParticipants.push_back(Participants[i]);
				Participants.erase(Participants.begin() + i);
				i = i - 1;
				break;
			}
		}		
	}

	// Recherche dans le dictionnaire des prénoms féminins
	for (unsigned int i = 0; i < Participants.size(); i++)
	{
		for (unsigned int j = 0; j < PrenomsFilles.size(); j++)
		{
			if (Participants[i].compare(PrenomsFilles[j]) == 0)
			{
				FillesParticipantes.push_back(Participants[i]);
				Participants.erase(Participants.begin() + i);
				i = i - 1;
				break;
			}
		}
	}

	// S'il reste des participants à trouver, on met les charactères spéciaux des noms en majuscule pour avoir plus de 'match' avec le dictionnaire
	if (Participants.size() > 0 && flag == 0)
	{
		for (unsigned int i = 0; i < Participants.size(); i++)
			Participants[i] = AccentsEnMajuscule(Participants[i]);
			
		flag = 1;
		goto recherchedictionnaire;  // finalement on retourne faire la recherche  
		// lors de mes tests, je pouvais trouvé 38 participants de plus sur 700 grâce à cela
	}

	// S'il reste des participants à trouver, on enlève les charactères spéciaux pour avoir plus de 'match' avec le dictionnaire
	if (Participants.size() > 0 && flag2 == 0)
	{
		for (unsigned int i = 0; i < Participants.size(); i++)
		Participants[i] = enleverAccents(Participants[i]);
		flag2 = 1;
		goto recherchedictionnaire;  
		
	}


	// S'il reste des participants à trouver, on propose la recherche en ligne avec L'API
	if (Participants.size() > 0)
	{
		cout << "Il y a " << Participants.size() << " personnes non trouver" << endl;

		while (true) {
			string reponse="";
			cout << endl << "Voulez vous tenter de trouver les personnes manquantes en ligne ? (Y/N)" << endl;
			getline(cin, reponse);
			if (reponse.length() == 1)
			{
				if (reponse == "y" || reponse == "Y")
				{
					VerificationNonTrouver(Monpointeur2, Monpointeur3, Monpointeur1, API1);
					break;
				}
				else
					break;
			}
			cout << "Invalid character, please try again" << endl;
		}
	}

	// S'il reste des participants à trouver, on propose de les classer manuellement
	if (Participants.size() > 0)
	{
		cout << "Il y a " << Participants.size() << " personnes toujours non trouver" << endl;

		while (true) {
			string reponse = "";
			cout << endl << "Voulez vous tenter de classer les personnes manuellement ? (Y/N)" << endl;
			cin.clear();
			getline(cin, reponse);
			if (reponse.length() == 1)
			{
				if (reponse == "y" || reponse == "Y")
				{
						for (unsigned int i = 0; i < Participants.size(); i++)
						{
							string response;
							cin.clear();
							cout << endl << Participants[i] << " est un gars ou une fille ? (1=Gars  2=Fille  3=Je ne sais pas)" << endl;
							cin >> response;

							if (response == "1")
							{
								GarsParticipants.push_back(Participants[i]);
								AjoutNomFichier(Participants[i],FichierGars);
								Participants.erase(Participants.begin() + i);
								i = i - 1;
								
							}
							else if (response == "2")
							{
								FillesParticipantes.push_back(Participants[i]);
								AjoutNomFichier(Participants[i], FicherFille);
								Participants.erase(Participants.begin() + i);
								i = i - 1;
							}
							else if (response == "3")	
							{
							}
							else
							{
								cout << "Entrée invalide";
								i = i - 1;
							}
						 }
						break;

					}
					else
						break;
				}
				else
					break;
			}
		}

	system("cls"); // Efface ce qui est affiché en console et on affiche les statistiques
	float gars = (float)GarsParticipants.size();
	float fille = (float)FillesParticipantes.size();
	float ratio = fille / gars;
	double pourcentageTrouver = (gars+fille) / (gars + fille +Participants.size());

	cout << "****************************************************************" << endl;
	cout << "** il y a " << FillesParticipantes.size() << " filles participantes";
	cout << " et " << GarsParticipants.size() << " gars participants   **" << endl << "** soit un ratio de " << ratio << "                                  **" << endl;
	cout << "** Trouver: " << GarsParticipants.size() + FillesParticipantes.size() << "/" << GarsParticipants.size() + FillesParticipantes.size() + Participants.size() << "(" 
		<< round(pourcentageTrouver * 100) << "%)                                     **" << endl;
	cout << "****************************************************************" << endl;

	ofstream myfile1;
	myfile1.open("nontrouve.txt");
	for (unsigned int i = 0; i < Participants.size(); i++)
	{
		myfile1 << Participants[i] << endl;
	}
	//myfile << "Writing this to a file.\n";
	myfile1.close();


	// Fin du programme
	system("pause");
	return 0;
}



vector<string> LectureFichier(string line, vector<string> p_myVector)
{

	ifstream myfile("attending.txt");
	if (myfile.is_open())
	{

		if (myfile.peek() == ifstream::traits_type::eof())
		{
			cout << "Aucun participant valide détecté !" << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}

		size_t found;

		while (getline(myfile, line))
		{

			if (line.find("name") != -1)
			{
				string nom = line.substr(15).c_str();
				nom.erase(nom.end() - 3);
				nom.erase(nom.end() - 2);
				found = nom.find(' ');
				nom.erase(found);
				transform(nom.begin(), nom.end(), nom.begin(),toupper); // majuscule
				p_myVector.push_back(nom);
			}
		}
		myfile.close();
		return p_myVector;
	}
	else
	{
		cout << "Aucun fichier de participants détecté !" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}


vector<string> LecturePrenom(string line, vector<string> p_myVector, string fichier)
{

	ifstream myfile(fichier);
	if (myfile.is_open())
	{

		if (myfile.peek() == ifstream::traits_type::eof())
		{
			cout << "Aucun prénom valide détecté !" << endl;
			exit(EXIT_FAILURE);
		}

		while (getline(myfile, line))
		{
			string prenom = line;
			p_myVector.push_back(prenom);			
		}
		myfile.close();

		return p_myVector;
	}
	else
	{
		cout << "Aucune fichier de prénom détecté !" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}


	
}

void AjoutNomFichier(string Nom, string Sexe)
{
	ofstream myfile(Sexe, std::ios_base::app | std::ios_base::out);
	if (myfile.is_open())
	{
		myfile << Nom << "\n";
		myfile.close();
	}
	else
		cout << "Erreur dans l'ouverture du fichier !" << endl;
}
void VerificationNonTrouver(vector<string>* FillesParticipantes, vector<string>* GarsParticipants, vector<string>* Participants,string API)
{

	vector<string> ParticipantsL = *Participants;
	vector<string> FillesParticipantesL = *FillesParticipantes;
	vector<string> GarsParticipantsL = *GarsParticipants;
	int watch = 0;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "WSAStartup failed.\n";
	}
	else
	{
		for (unsigned int i = 0; i< ParticipantsL.size(); i++)
		{
			SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			struct hostent *host;
//			host = gethostbyname("gender-api.com");
			host = gethostbyname("api.genderize.io");
			SOCKADDR_IN SockAddr;
			SockAddr.sin_port = htons(80);
			SockAddr.sin_family = AF_INET;
			SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
			if (watch == 0)
				cout << "Connecting...\n";
			if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0){
				cout << "Could not connect";
			}
			else
			{
				if (watch == 0)
					cout << "Connected.\n";
				watch = 1;

				string data;

				//ParticipantsL[i] = enleverAccents(ParticipantsL[i]);

				string request = "GET /?name=" + ParticipantsL[i] + " HTTP / 1.1\r\nHost: api.genderize.io\r\nConnection: close\r\n\r\n";
	//			string request = "GET /get?name=" + ParticipantsL[i] + " HTTP / 1.1\r\nHost: gender-api.com\r\nConnection: close\r\n\r\n";
				send(Socket, request.c_str(), request.length(), 0);

				char buffer[100];
				buffer[100];

				int nDataLength;

				while ((nDataLength = recv(Socket, buffer, 100, 0)) > 0){
					data = buffer;
				}
				
				if (data.find("null") != -1)      // Inconnu par l'API
						cout << ParticipantsL[i] << " inconnu par l'API" << endl;
				else if (data.find("female") != -1)  // C'est une fille
				{
					cout << ParticipantsL[i] << " est une fille" << endl;
					AjoutNomFichier(ParticipantsL[i], FicherFille);
					FillesParticipantesL.push_back(ParticipantsL[i]);
					ParticipantsL.erase(ParticipantsL.begin() + i);
					i = i - 1;
					//break;
				}
				else if (data.find("male") != -1)                      // C'est un gars
				{
					cout << ParticipantsL[i] << " est un gars" << endl;
					AjoutNomFichier(ParticipantsL[i], FichierGars);
					GarsParticipantsL.push_back(ParticipantsL[i]);
					ParticipantsL.erase(ParticipantsL.begin() + i);
					i = i - 1;
					//break;
				}
				else
					cout << "Erreur avec le nom : " << ParticipantsL[i] << endl;
				}
				closesocket(Socket);			
		}
		WSACleanup();
	}
	
	*Participants = ParticipantsL;
	*FillesParticipantes = FillesParticipantesL;
	*GarsParticipants = GarsParticipantsL;
}


string enleverAccents(string nom)
{
	string Nom = nom;

	char IllegalCharactersE[] = { 'é', 'É', 'È', 'è', 'ë', 'Ë' };
	char IllegalCharactersO[] = { 'ô', 'Ö', 'Ô', 'ö' };
	char IllegalCharactersi[] = { 'í', 'î', 'Ï', 'ï', 'Î' };
	char IllegalCharactersC[] = { 'ç', 'Ç' };

	for (unsigned int i = 0; i < Nom.length(); i++)
	{
		for (int j = 0; j < sizeof(IllegalCharactersE); j++)
				if (Nom[i] == IllegalCharactersE[j])
					Nom[i] = 'E';

		for (int j = 0; j < sizeof(IllegalCharactersO); j++)
			if (Nom[i] == IllegalCharactersO[j])
				Nom[i] = 'O';

		for (int j = 0; j < sizeof(IllegalCharactersi); j++)
			if (Nom[i] == IllegalCharactersi[j])
				Nom[i] = 'I';

		for (int j = 0; j < sizeof(IllegalCharactersC); j++)
			if (Nom[i] == IllegalCharactersC[j])
				Nom[i] = 'C';
	}
	return Nom;
}

string AccentsEnMajuscule(string nom)
{
	string Nom = nom;

	char CharactersEgrave[] = { 'è' };
	char CharactersEaigu[] = { 'é' };
	char CharactersEtremas[] = {  'ë' };
	char CharactersO1[] = { 'ô' };
	char CharactersO2[] = { 'ö' };
	char Charactersi[] = { 'î'};
	char CharactersC[] = { 'ç' };

	for (unsigned int i = 0; i < Nom.length(); i++)
	{
		for (int j = 0; j < sizeof(CharactersEgrave); j++)
			if (Nom[i] == CharactersEgrave[j])
				Nom[i] = 'È';

		for (int j = 0; j < sizeof(CharactersEaigu); j++)
			if (Nom[i] == CharactersEaigu[j])
				Nom[i] = 'É';

		for (int j = 0; j < sizeof(CharactersEtremas); j++)
			if (Nom[i] == CharactersEtremas[j])
				Nom[i] = 'Ë';

		for (int j = 0; j < sizeof(CharactersO1); j++)	
			if (Nom[i] == CharactersO1[j])
				Nom[i] = 'Ô';

		for (int j = 0; j < sizeof(CharactersO2); j++)
			if (Nom[i] == CharactersO2[j])
				Nom[i] = 'Ö';

		for (int j = 0; j < sizeof(Charactersi); j++)
			if (Nom[i] == Charactersi[j])
				Nom[i] = 'Î';

		for (int j = 0; j < sizeof(CharactersC); j++)
			if (Nom[i] == CharactersC[j])
				Nom[i] = 'Ç';
	}
	return Nom;
}