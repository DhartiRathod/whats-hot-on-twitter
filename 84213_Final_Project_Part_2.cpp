/*
SEN 909 - OO PROGRAMMING WITH C++

Final Project - Part-2
Assignment Group:  Tweeter

Name		: DHARTI RATHOD
Student ID	: 84213

Web Page Link: http://dhartikrunal.wix.com/tweetanalyzer

*/


#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>

using namespace std;

// Data class to store element data
template<class TYPE>
class Data{
	public:
		long key;
		int sentimentScore;
		TYPE message; // Text message from the tweet
		TYPE location; // location of user who tweeted
		TYPE created_at; // time when user tweeted this message
		
		
		// Default constructor
		Data(): key(0), sentimentScore(0), message(""), location(""), created_at("")
		{}
		
		// Constructor with two arguments: can be used for both - slot and element.
		Data(int s, TYPE s2, TYPE s3, TYPE s4)
		{
			key = 0;
			sentimentScore = s;
			message = s2;
			location = s3;
			created_at = s4;
		}
};

// 'Data_slot' class to hold the Slot data
template<class TYPE>
class Data_slot{
	public:
		TYPE index;
		long numberOfElements;
		int totalPositiveSentiments;
		int totalNeutralSentiments;
		int totalNegetiveSentiments;
		
		// Default constructor
		Data_slot(): index(""), numberOfElements(0), totalPositiveSentiments(0), totalNeutralSentiments(0), totalNegetiveSentiments(0)
		{}
		// Constructor with argument
		Data_slot(TYPE s) : numberOfElements(0)  // When we create a slot, it will have no elements initially.
		{
			totalPositiveSentiments = 0;
			totalNeutralSentiments = 0;
			totalNegetiveSentiments = 0;
			index = s;
		}
};


// Element class
template<class TYPE>
class Element
{
	public:
		Data<TYPE> d;
		Element<TYPE>* prevElement;
		Element<TYPE>* nextElement;
		// Default constructor
		Element(): prevElement(NULL), nextElement(NULL)
		{	
			d.key = 0;
			d.sentimentScore = 0;
			d.message = "";
			d.location = "";
			d.created_at = "";
		}
		// Constructor with arguments
		Element(Data<TYPE> d1): prevElement(NULL), nextElement(NULL)
		{
			d = d1;
		}
		// Displays the contents of the current Element.
		void showElement(){
			cout<<" \t\t ElementData: -------------------------"<<endl;
			cout<<" \t\t key: 		 "<<d.key<<endl;
			cout<<" \t\t sentimentScore: 		 "<<d.sentimentScore<<endl;
			cout<<" \t\t message: 	 "<<d.message<<endl;
			cout<<" \t\t location: 	 "<<d.location<<endl;
			cout<<" \t\t created_at: "<<d.created_at<<endl;
			cout<<"------------------------------------------------"<<endl;
			
		}
};



// 'Slot' class
template<class TYPE>
class Slot
{ 
	public:
		Data_slot<TYPE> d;
		Slot<TYPE>* prevSlot;
		Element<TYPE>* headElement;
		Slot<TYPE>* nextSlot;
		// Default constructor
		Slot(): prevSlot(NULL), nextSlot(NULL), headElement(NULL)
		{
			d.index = "";
			d.numberOfElements = 0;
			d.totalPositiveSentiments = 0;
			d.totalNeutralSentiments = 0;
			d.totalNegetiveSentiments = 0;
		}
		// Constructor with arguments
		Slot(Data_slot<TYPE> d1): prevSlot(NULL), nextSlot(NULL), headElement(NULL)
		{
			d = d1;
		}
		
		//insert element pointed by x at the beginning of list of elements
		void insert(Element<TYPE>* x)
		{
			if(this->headElement != NULL){  // If there is at-least one element in the slot s, then only need to set prev and next pointers for elements.  
				x->nextElement = this->headElement;
				this->headElement->prevElement = x;
			}
			this->headElement = x;
			
			d.numberOfElements++;
			if(x->d.sentimentScore > 0){
				d.totalPositiveSentiments++;
			}else if(x->d.sentimentScore < 0){
				d.totalNegetiveSentiments++;
			}else{
				d.totalNeutralSentiments++;
			}
			x->d.key = d.numberOfElements;
		}
		
		//remove element pointed by x from list of elements
		void deleteElement(Element<TYPE>* x)
		{
			if(x->nextElement != NULL){
					x->nextElement->prevElement = x->prevElement; // No need to execute this for the last Element
			}
			if(x->prevElement != NULL){
				x->prevElement->nextElement = x->nextElement; // No need to execute this for the first Element
			}else{
				this->headElement = x->nextElement; // We are deleting first Element from the list, so need to move "headElement" pointer.
			}
			delete x;                        //deallocate x
			d.numberOfElements--;
		}
		
		
		// Displays the contents of the current Slot's index node.
		void showSlotData(){
			cout<<"SlotData: '"<<d.index<<"'. Total Elements: "<<d.numberOfElements<<endl;
			cout<<" - Total Positive Sentiments: "<<d.totalPositiveSentiments<<endl;
			cout<<" - Total Neutral Sentiments: "<<d.totalNeutralSentiments<<endl;
			cout<<" - Total Negetive Sentiments: "<<d.totalNegetiveSentiments<<endl;
			cout<<"**************************************************************"<<endl;
		}
		
		//destructor
		~Slot()
		{ 
			while (this->headElement != NULL) 
				deleteElement(this->headElement); 
		}
};

template<class TYPE>
class HashMap
{
	private:
		long size;
		Slot<TYPE>* first; // A pointer to a Slot
	public:
		HashMap(){
			first = NULL;  // Initialize the HashMap with no Slots
		}
		
		// Returns the total number of Slots in the Hashmap
		// Traverse through all Slots. Maintains and return the count of Slots
		int getSize(){
			int count = 0;
			Slot<TYPE>* temp = first;
			while(temp != NULL){ 
				count++;
				temp = temp->nextSlot;
			}
			return count;
		}
		
		// Inserts the Slot pointed by 's' at the beginning of the HashMap.
		void insert(Slot<TYPE>* s){
			if(this->first != NULL){  // If there is at-least one slot in the HashMap 
				s->nextSlot = this->first;
				this->first->prevSlot = s;
			}
			this->first = s;
		}
		
		// Inserts the Element pointed by 'x' at the beginning of the list pointed by the Slot<TYPE> which is pointed by 's'
		void insert(Slot<TYPE>* s, Element<TYPE>* x){
			s->insert(x);
		}
		
		// Deletes the whole Slot pointed by 's' from the HashMap
		void deleteSlot(Slot<TYPE>* s){
			if((first != NULL) || (s != NULL)){ // Check the base conditions: If HashMap is empty or s is NULL, nothing to do.
				if(s->prevSlot != NULL){
					s->prevSlot->nextSlot = s->nextSlot; // No need to execute this for first Slot
				}else{
					first = s->nextSlot;   // We are deleting first Slot, so need to move "first" pointer.
				}
				
				if(s->nextSlot != NULL){
					s->nextSlot->prevSlot = s->prevSlot; // No need to execute this for last Slot
				}
				
				while (s->headElement != NULL)             //delete all elements from slot s
					s->deleteElement(s->headElement);
				
				delete s;                           //deallocate s
			}
		}
		
		// Deletes the Element pointed by 'x' from the list pointed by the Slot pointed by 's'.
		void deleteElement(Slot<TYPE>* s, Element<TYPE>* x){
			s->deleteElement(x);
		}
		
		// Extra function for troubleshooting 
		void showHashMap() const
		{
			Slot<TYPE>* temp = first;
			while(temp != NULL){
				temp->showSlotData();
				Element<TYPE>* tempE = temp->headElement;
				while(tempE != NULL){
					tempE->showElement();
					tempE = tempE->nextElement;
				}
				cout<<endl;
				temp = temp->nextSlot;
			}
		}
		
		// Prints the data from Hashmap according to user's input.
		void showCustomeDataFromHashMap() const
		{
			char userInput[30];
			do{
				// Print available format in which user can provide his'her input.
				cout<<"What would like to print?"<<endl;
				cout<<"To print all slot indices, enter the string 's all'"<<endl;
				cout<<"To print the n first slot indices, where n is a non-negative integer, enter the string 's n'"<<endl;
				cout<<"To print all element keys in slot <slot_id> enter the string 's <slot_id> k all'"<<endl;
				cout<<"To print the n first element keys in slot <slot_id>, where n is a non-negative integer, enter the string 's <slot_id> k n'"<<endl;
				cout<<"To print all the associated information about key <key_id> in slot <slot_id> enter the string 's <slot_id> k <key_id>'"<<endl;
				cout<<"To EXIT enter 'e'"<<endl;
				cin.getline(userInput,30);
				
				// If first character is NOT 's', that means either user want to exit or it is an invalid input.
				if(userInput[0] == 's'){
					string userInputString(userInput);
					istringstream stream1;
					stream1.str(userInputString);
					string options[10];
					int size = 0;
					
					// Extract all options.
					while(stream1 >> options[size]){
						size++;
					}
					
					if(size == 2){  // For "s all" and "s n"
						if(options[1] == "all"){
							// Print all slot
							Slot<TYPE>* temp = first;
							while(temp != NULL){
								temp->showSlotData();
								cout<<endl;
								temp = temp->nextSlot;
							}
						}else if(isdigit(options[1].at(0))){
							int n = atoi(options[1].c_str());
							int k = 1;
							// Print first 'n' slots
							Slot<TYPE>* temp = first;
							while((temp != NULL)&&(k<=n)){
								temp->showSlotData();
								cout<<endl;
								temp = temp->nextSlot;
								k++;
							}
						}else{
							cout<<"Invalid options entered. Please try again...."<<endl<<endl<<endl;
						}
					}else if(size == 4){ // For last three types of input.
						string slotId = options[1];
						// Print all Elements from the Slot='slotId'
						if(options[3] == "all"){
							Slot<TYPE>* temp = first;
							while(temp != NULL){
								if(temp->d.index == slotId){
									temp->showSlotData();
									Element<TYPE>* tempE = temp->headElement;
									while(tempE != NULL){
										tempE->showElement();
										tempE = tempE->nextElement;
									}
									cout<<endl;
								}								
								temp = temp->nextSlot;
							}
						}else if(isdigit(options[3].at(0))){
						// Print first 'n' Elements from the Slot='slotId'
							int n = atoi(options[3].c_str());
							int k = 1;
							Slot<TYPE>* temp = first;
							while(temp != NULL){
								if(temp->d.index == slotId){
									temp->showSlotData();
									Element<TYPE>* tempE = temp->headElement;
									while((tempE != NULL)&&(k<=n)){
										tempE->showElement();
										tempE = tempE->nextElement;
										k++;
									}
									cout<<endl;
								}								
								temp = temp->nextSlot;
							}
						}else{
							// Print a specific Element(with keyId) from the Slot='slotId'
							long keyId = atol(options[3].c_str());
							Slot<TYPE>* temp = first;
							while(temp != NULL){
								if(temp->d.index == slotId){
									temp->showSlotData();
									Element<TYPE>* tempE = temp->headElement;
									while(tempE != NULL){
										if(tempE->d.key == keyId){
											tempE->showElement();
										}
										tempE = tempE->nextElement;
									}
									cout<<endl;
								}								
								temp = temp->nextSlot;
							}
						}
					}else{
						cout<<"Invalid options entered. Please try again...."<<endl<<endl<<endl;
					}
				}
			}while(userInput[0] != 'e');
		}
		
		void sortSlotsByNumberOfElements(){
			Slot<TYPE>* s1;
			Slot<TYPE>* s2;
			Slot<TYPE>* s1next;
			
			for(s1 = first; s1->nextSlot != NULL; s1 = s1->nextSlot) {
				
				for( s2 = s1->nextSlot; s2 != NULL; s2 = s2->nextSlot) {
					
					if(s2->d.numberOfElements > s1->d.numberOfElements) {
						Data_slot<TYPE> tempData;
						tempData = s1->d;
						s1->d = s2->d;
						s2->d = tempData;
						
						Element<TYPE>* tempHeadElement;
						tempHeadElement = s1->headElement;
						s1->headElement = s2->headElement;
						s2->headElement = tempHeadElement;
					}
				}
			  }
		}
		
		// Searches for an index in all slots. If found, returns the link to that slot, else, returns NULL.
		Slot<TYPE>* wordExistInHashMap(string word){
			Slot<TYPE>* temp = first;
			bool found = false;
			while(temp!=NULL){
				if(temp->d.index == word){
					found = true;
					break;
				}
				temp = temp->nextSlot;
			}
			return temp;
		}
		
		//destructor
		~HashMap()
		{
			while (this->first != NULL)
				this->deleteSlot(this->first);
		}
		
};

	// Helper functions:

	// Extracts the value of a filed from the json text.
	// This is a very basic level implementation, it would work with the tweeter json only.
	string getFieldValueFromJson(string tweet, string field, string endDelimeter){
		string s;
		size_t pos = 0;
		pos = tweet.find("\""+field+"\":");
		
		if (pos != std::string::npos){	
			s = tweet.substr(pos+ field.length()+3);
			
			pos = s.find(endDelimeter);
			if (pos != std::string::npos){
				s = s.substr(0, pos);
			}
		}
		
		return s;
	}

	
	int getSentimentScore(string word, string sentimentDictionary[10000][2]){
		int i = 0;
		int score = 0;
		while(!sentimentDictionary[i][0].empty()){
			if(sentimentDictionary[i][0].compare(word) == 0){
				
				if(sentimentDictionary[i][1].compare("negative") == 0){
					score = -1;
				}else if(sentimentDictionary[i][1].compare("positive") == 0){
					score = 1;
				}else if(sentimentDictionary[i][1].compare("neutral") == 0){
					score = 0;
				}
			}
			i++;
		}
		// cout<<"'"<<sentimentDictionary[0][0]<<"'"<<endl;
		// cout<<"'"<<word<<"'\t\t"<<score<<endl;
		
		return score;
	}
	
	
	
	
	// Seperates words from a given text message and stores them into a String Array
	int extractWordsAndSentimentFromMessage(string text, string words[100], int& size, string sentimentDictionary[10000][2]){
		int sentimentScore = 0;
		istringstream stream1;
		stream1.str(text);
		while(stream1 >> words[size]){
			sentimentScore = sentimentScore + getSentimentScore(words[size], sentimentDictionary);
			size++;
		}
		return sentimentScore;
	}
	
	
	
	
	// Cleans up the text message by  (Need to add more words to be cleaned up)
	// - Converts all characters to lower case
	// - Removes unwanted characters like ',",:,....
	// - Removes unwanted words like a, an, the.....
	string cleaupText(string text){
		
		for(int i=0; i<text.size();++i)
			text[i] = tolower(text[i]);
		size_t pos = 0;
		int i = 0;
		
		string specialChars[100] = {"'", ".", ":", "\"", "\\t", "\\n", ")", "(", "-", "!", ";"}; 
		
		while(!specialChars[i].empty()){
			while ((pos = text.find(specialChars[i])) != std::string::npos){
				text.erase(pos,specialChars[i].length());
			}
			i++;
		}
		
		
		string unwantedWords[800] = {" rt "," al "," &amp "," se "," el "," ver "," en "," la "," de ", "im ", "i "," mrs "," miss "," mr "," jr ", " i "," a "," a's "," able "," about "," above "," according "," accordingly "," across "," actually "," after "," afterwards "," again "," against "," ain't "," all "," allow "," allows "," almost "," alone "," along "," already "," also "," although "," always "," am "," among "," amongst "," an "," and "," another "," any "," anybody "," anyhow "," anyone "," anything "," anyway "," anyways "," anywhere "," apart "," appear "," appreciate "," appropriate "," are "," aren't "," around "," as "," aside "," ask "," asking "," associated "," at "," available "," away "," awfully "," b "," be "," became "," because "," become "," becomes "," becoming "," been "," before "," beforehand "," behind "," being "," believe "," below "," beside "," besides "," best "," better "," between "," beyond "," both "," brief "," but "," by "," c "," c'mon "," c's "," came "," can "," can't "," cannot "," cant "," cause "," causes "," certain "," certainly "," changes "," clearly "," co "," com "," come "," comes "," concerning "," consequently "," consider "," considering "," contain "," containing "," contains "," corresponding "," could "," couldn't "," course "," currently "," d "," definitely "," described "," despite "," did "," didn't "," different "," do "," does "," doesn't "," doing "," don't "," done "," down "," downwards "," during "," e "," each "," edu "," eg "," eight "," either "," else "," elsewhere "," enough "," entirely "," especially "," et "," etc "," even "," ever "," every "," everybody "," everyone "," everything "," everywhere "," ex "," exactly "," example "," except "," f "," far "," few "," fifth "," first "," five "," followed "," following "," follows "," for "," former "," formerly "," forth "," four "," from "," further "," furthermore "," g "," get "," gets "," getting "," given "," gives "," go "," goes "," going "," gone "," got "," gotten "," greetings "," h "," had "," hadn't "," happens "," hardly "," has "," hasn't "," have "," haven't "," having "," he "," he's "," hello "," help "," hence "," her "," here "," here's "," hereafter "," hereby "," herein "," hereupon "," hers "," herself "," hi "," him "," himself "," his "," hither "," hopefully "," how "," howbeit "," however "," i "," i'd "," i'll "," i'm "," i've "," ie "," if "," ignored "," immediate "," in "," inasmuch "," inc "," indeed "," indicate "," indicated "," indicates "," inner "," insofar "," instead "," into "," inward "," is "," isn't "," it "," it'd "," it'll "," it's "," its "," itself "," j "," just "," k "," keep "," keeps "," kept "," know "," knows "," known "," l "," last "," lately "," later "," latter "," latterly "," least "," less "," lest "," let "," let's "," like "," liked "," likely "," little "," look "," looking "," looks "," ltd "," m "," mainly "," many "," may "," maybe "," me "," mean "," meanwhile "," merely "," might "," more "," moreover "," most "," mostly "," much "," must "," my "," myself "," n "," name "," namely "," nd "," near "," nearly "," necessary "," need "," needs "," neither "," never "," nevertheless "," new "," next "," nine "," no "," nobody "," non "," none "," noone "," nor "," normally "," not "," nothing "," novel "," now "," nowhere "," o "," obviously "," of "," off "," often "," oh "," ok "," okay "," old "," on "," once "," one "," ones "," only "," onto "," or "," other "," others "," otherwise "," ought "," our "," ours "," ourselves "," out "," outside "," over "," overall "," own "," p "," particular "," particularly "," per "," perhaps "," placed "," please "," plus "," possible "," presumably "," probably "," provides "," q "," que "," quite "," qv "," r "," rather "," rd "," re "," really "," reasonably "," regarding "," regardless "," regards "," relatively "," respectively "," right "," s "," said "," same "," saw "," say "," saying "," says "," second "," secondly "," see "," seeing "," seem "," seemed "," seeming "," seems "," seen "," self "," selves "," sensible "," sent "," serious "," seriously "," seven "," several "," shall "," she "," should "," shouldn't "," since "," six "," so "," some "," somebody "," somehow "," someone "," something "," sometime "," sometimes "," somewhat "," somewhere "," soon "," sorry "," specified "," specify "," specifying "," still "," sub "," such "," sup "," sure "," t "," t's "," take "," taken "," tell "," tends "," th "," than "," thank "," thanks "," thanx "," that "," that's "," thats "," the "," their "," theirs "," them "," themselves "," then "," thence "," there "," there's "," thereafter "," thereby "," therefore "," therein "," theres "," thereupon "," these "," they "," they'd "," they'll "," they're "," they've "," think "," third "," this "," thorough "," thoroughly "," those "," though "," three "," through "," throughout "," thru "," thus "," to "," together "," too "," took "," toward "," towards "," tried "," tries "," truly "," try "," trying "," twice "," two "," u "," un "," under "," unfortunately "," unless "," unlikely "," until "," unto "," up "," upon "," us "," use "," used "," useful "," uses "," using "," usually "," uucp "," v "," value "," various "," very "," via "," viz "," vs "," w "," want "," wants "," was "," wasn't "," way "," we "," we'd "," we'll "," we're "," we've "," welcome "," well "," went "," were "," weren't "," what "," what's "," whatever "," when "," whence "," whenever "," where "," where's "," whereafter "," whereas "," whereby "," wherein "," whereupon "," wherever "," whether "," which "," while "," whither "," who "," who's "," whoever "," whole "," whom "," whose "," why "," will "," willing "," wish "," with "," within "," without "," won't "," wonder "," would "," would "," wouldn't "," x "," y "," yes "," yet "," you "," you'd "," you'll "," you're "," you've "," your "," yours "," yourself "," yourselves "," z "," zero "};
		
		i = 0;
		while(!unwantedWords[i].empty()){
			// cout<<"'"<<unwantedWords[i]<<"'"<<endl;
		
			while ((pos = text.find(unwantedWords[i])) != std::string::npos){
				text.erase(pos+1,unwantedWords[i].length()-1);
			}
			i++;
		}
		
		return text;
	}

	void loadSentimentDictionary(string sentimentDictionary[10000][2]){
		string line;
		int i = 0;
		// Open the dictionary file
		std::ifstream infile("dictionary.txt");
		
		// Read each line from the file
		while (!infile.eof()) 
		{
			// Read each line from the file(which is an entire Tweet)
			std::getline(infile, line);
			istringstream stream1;
			stream1.str(line);
			stream1 >> sentimentDictionary[i][0];
			stream1 >> sentimentDictionary[i][1];
			i++;
		}
		
		// i=0;
		// while(!sentimentDictionary[i][0].empty()){
			// cout<<sentimentDictionary[i][0]<<"\t"<<sentimentDictionary[i][1]<<endl;
			// i++;
			// if(i == 20) break;
		// }
	}

	int main(){
		
		string sentimentDictionary[10000][2];
		loadSentimentDictionary(sentimentDictionary);
		
		// Intantiate a Hashmap
		HashMap<string>* h1 = new HashMap<string>();
		
		string tweet;
		// Open the input file
		// std::ifstream infile("FlumeData.txt");
		std::ifstream infile("sample.json");
		
		int i =1;
		// Read each line from the file(which is an entire Tweet)
		while (!infile.eof()) 
		{
			cout<<i<<endl;
			i++;
			// Read each line from the file(which is an entire Tweet)
			std::getline(infile, tweet);
			
			// Extract desired filed values from the Tweet Json.
			string message = getFieldValueFromJson(tweet, "text", "\",\"geo");
			message = cleaupText(message);
			
			string retweeted = getFieldValueFromJson(tweet, "retweeted", ",\"in_reply_to_screen_name");
			string location = getFieldValueFromJson(tweet, "location", "\",\"default_profile");
			// string tweetId = getFieldValueFromJson(tweet, "id", ",\"source");
			string created_at = getFieldValueFromJson(tweet, "created_at", "\",\"in_reply_to_user_id");
			
			string words[100];
			int size = 0;
			int sentimentScore = 0;
			
			// Extract Words from the tweet message.
			sentimentScore = extractWordsAndSentimentFromMessage(message, words, size, sentimentDictionary);
			
			// For each word,
			// If the word exists in any Slot of the hashmap, insert a new Element with desired data from the current Tweet.
			// Else, Inser a new Slot with this word and insert a new Element with desired data from the current Tweet.
			for(int i=0; i<size; i++){
				Slot<string> *temp = h1->wordExistInHashMap(words[i]); // Check if word already exists in the HashMap
				if(temp!=NULL){
					// Create new Element and insert into Slot
					Data<string> d(sentimentScore, message, location,created_at);
					Element<string> *e;
					try
					{ e = new Element<string>(d);}
					catch(bad_alloc) //exception handler
					{
						cout << " bad_alloc exception: can’t allocate memory for Element - e."<<endl;
						return(1);
					}
					h1->insert(temp, e);
				}else{
					// Create a Slot
					Data_slot<string> ds(words[i]);
					// ds.addSentimentScore(sentimentScore);
					Slot<string> *newSlot;
					try
					{
						newSlot = new Slot<string>(ds);
					}
					catch(bad_alloc) //exception handler
					{
						cout << " bad_alloc exception: can’t allocate memory for Slot."<<endl;
						return(1);
					}
					h1->insert(newSlot);
					
					// Create new Element and insert into Slot
					Data<string> d(sentimentScore, message, location,created_at);
					Element<string> *e;
					try
					{
						e = new Element<string>(d);
					}
					catch(bad_alloc) //exception handler
					{
						cout << " bad_alloc exception: can’t allocate memory for Element - e."<<endl;
						return(1);
					}
					h1->insert(newSlot, e);
				}
			}
		}
		
		h1->sortSlotsByNumberOfElements();
		// Ask user what data would he/She like to see and print data accordingly.
		h1->showCustomeDataFromHashMap();
		
		// Delete the HashMap
		delete h1;
		
		return 0;
}