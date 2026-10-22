
#include <iostream>
#include <string>
using namespace std;

// Node for Doubly Linked List
struct Song {
    string title;
    string artist;
    string album;
    float duration;

    Song* prev;
    Song* next;
};

// Separate node for Recently Played Stack
struct RecentNode {
    string title;
    string artist;
    string album;
    float duration;

    RecentNode* next;
};

class Playlist {
private:
    Song* head;
    Song* tail;
    Song* current;

    RecentNode* recentTop;
    Song* queueFront;
    Song* queueRear;

    // Add a song to the Recently Played Stack
    void pushRecent(Song* song) {
        RecentNode* newNode = new RecentNode;

        newNode->title = song->title;
        newNode->artist = song->artist;
        newNode->album = song->album;
        newNode->duration = song->duration;

        newNode->next = recentTop;
        recentTop = newNode;
    }

public:
    // Constructor
    Playlist() {
        head = NULL;
        tail = NULL;
        current = NULL;
        recentTop = NULL;
        queueFront = NULL;
        queueRear = NULL;
    }

    // Destructor: free dynamically allocated memory
    
    // Destructor: free dynamically allocated memory
    ~Playlist() {
        // Free playlist nodes
        while (head != NULL) {
            Song* temp = head;
            head = head->next;
            delete temp;
        }

        // Free recently played stack nodes
        while (recentTop != NULL) {
            RecentNode* temp = recentTop;
            recentTop = recentTop->next;
            delete temp;
        }

        // Free upcoming queue nodes
        while (queueFront != NULL) {
            Song* temp = queueFront;
            queueFront = queueFront->next;
            delete temp;
        }

        queueRear = NULL;
    }


    // 1. Add Song
    void addSong() {
        string title, artist, album;
        float duration;

        cout << "\nEnter song title: ";
        getline(cin >> ws, title);

        cout << "Enter artist name: ";
        getline(cin, artist);

        cout << "Enter album name: ";
        getline(cin, album);

        cout << "Enter duration in minutes: ";

        if (!(cin >> duration) || duration <= 0) {
            cout << "Invalid duration!\n";
            cin.clear();
            cin.ignore(10000, '\n');
            return;
        }

        Song* newSong = new Song;

        newSong->title = title;
        newSong->artist = artist;
        newSong->album = album;
        newSong->duration = duration;

        newSong->prev = tail;
        newSong->next = NULL;

        if (head == NULL) {
            head = tail = newSong;
        } else {
            tail->next = newSong;
            tail = newSong;
        }

        cout << "\nSong added successfully!\n";
    }

    
    // 9. Add Song to Queue
    void addToQueue()
     {
        if (head == NULL) {
            cout << "\nPlaylist is empty! Add songs first.\n";
            return;
        }

        string title;
        cout << "\nEnter song title to add to queue: ";
        getline(cin >> ws, title);

        // Find song in the main playlist
        Song* temp = head;

        while (temp != NULL && temp->title != title) {
            temp = temp->next;
        }

        if (temp == NULL) {
            cout << "\nSong not found in playlist!\n";
            return;
        }

        // Create a separate queue node
        Song* newNode = new Song;
        newNode->title = temp->title;
        newNode->artist = temp->artist;
        newNode->album = temp->album;
        newNode->duration = temp->duration;
        newNode->prev = queueRear;
        newNode->next = NULL;

        // Add node at the rear of the queue
        if (queueFront == NULL) {
            queueFront = queueRear = newNode;
        } else {
            queueRear->next = newNode;
            queueRear = newNode;
        }

        cout << "\nSong added to upcoming queue successfully!\n";
    }


    // 2. Display Playlist
    void displayPlaylist() {
        if (head == NULL) {
            cout << "\nPlaylist is empty!\n";
            return;
        }

        Song* temp = head;
        int count = 1;

        cout << "\n MY PLAYLIST \n";

        while (temp != NULL) {
            cout << "\nSong " << count++ << endl;
            cout << "Title    : " << temp->title << endl;
            cout << "Artist   : " << temp->artist << endl;
            cout << "Album    : " << temp->album << endl;
            cout << "Duration : " << temp->duration
                 << " minutes" << endl;

            if (temp == current) {
                cout << "Status   : Current Song" << endl;
            }

            
            temp = temp->next;
        }
    }

    // 3. Delete Song
    void deleteSong() {
        if (head == NULL) {
            cout << "\nPlaylist is empty!\n";
            return;
        }

        string title;
        cout << "\nEnter song title to delete: ";
        getline(cin >> ws, title);

        Song* temp = head;

        while (temp != NULL && temp->title != title) {
            temp = temp->next;
        }

        if (temp == NULL) {
            cout << "\nSong not found!\n";
            return;
        }

        // Keep current pointer valid if current song is deleted
        if (current == temp) {
            current = (temp->next != NULL)
                    ? temp->next : temp->prev;
        }

        if (temp->prev != NULL) {
            temp->prev->next = temp->next;
        } else {
            head = temp->next;
        }

        if (temp->next != NULL) {
            temp->next->prev = temp->prev;
        } else {
            tail = temp->prev;
        }

        delete temp;

        cout << "\nSong deleted successfully!\n";
    }

    // 4. Play Song
    void playSong() {
        if (head == NULL) {
            cout << "\nPlaylist is empty!\n";
            return;
        }

        string title;
        cout << "\nEnter song title to play: ";
        getline(cin >> ws, title);

        Song* temp = head;

        while (temp != NULL && temp->title != title) {
            temp = temp->next;
        }

        if (temp == NULL) {
            cout << "\nSong not found!\n";
            return;
        }

        current = temp;
        pushRecent(current);

        cout << "\nNow Playing: " << current->title << endl;
        cout << "Artist: " << current->artist << endl;
        cout << "Album : " << current->album << endl;
    }

    // 5. Next Song
    void nextSong() {
        if (head == NULL) {
            cout << "\nPlaylist is empty!\n";
            return;
        }

        if (current == NULL) {
            current = head;
        } else if (current->next != NULL) {
            current = current->next;
        } else {
            cout << "\nAlready at the last song!\n";
            return;
        }

        pushRecent(current);

        cout << "\nNow Playing: " << current->title << endl;
        cout << "Artist: " << current->artist << endl;
    }

    // 6. Previous Song
    void previousSong() {
        if (head == NULL) {
            cout << "\nPlaylist is empty!\n";
            return;
        }

        if (current == NULL) {
            current = head;
        } else if (current->prev != NULL) {
            current = current->prev;
        } else {
            cout << "\nAlready at the first song!\n";
            return;
        }

        pushRecent(current);

        cout << "\nNow Playing: " << current->title << endl;
        cout << "Artist: " << current->artist << endl;
    }

    // 7. Search Song
    void searchSong() {
        if (head == NULL) {
            cout << "\nPlaylist is empty!\n";
            return;
        }

        string title;
        cout << "\nEnter song title to search: ";
        getline(cin >> ws, title);

        Song* temp = head;

        while (temp != NULL) {
            if (temp->title == title) {
                cout << "\n SONG FOUND \n";
                cout << "Title    : " << temp->title << endl;
                cout << "Artist   : " << temp->artist << endl;
                cout << "Album    : " << temp->album << endl;
                cout << "Duration : " << temp->duration
                     << " minutes" << endl;
                cout << "================================\n";
                return;
            }

            temp = temp->next;
        }

        cout << "\nSong not found!\n";
    }

    // 8. Show Recently Played Stack
    void showRecentlyPlayed() {
        if (recentTop == NULL) {
            cout << "\nNo recently played songs.\n";
            return;
        }

        RecentNode* temp = recentTop;
        int count = 1;

        cout << "\n RECENTLY PLAYED (STACK) \n";

        while (temp != NULL) {
            cout << "\n" << count++ << ". " << temp->title << endl;
            cout << "Artist : " << temp->artist << endl;
            cout << "Album  : " << temp->album << endl;
            cout << "-----------------------------------\n";

            temp = temp->next;
        }
    }
};

int main() {
    Playlist playlist;
    int choice;

    do {
        cout << "       MUSIC PLAYLIST MANAGER\n";
        cout << "1. Add Song\n";
        cout << "2. Display Playlist\n";
        cout << "3. Delete Song\n";
        cout << "4. Play Song\n";
        cout << "5. Next Song\n";
        cout << "6. Previous Song\n";
        cout << "7. Search Song\n";
        cout << "8. Show Recently Played\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "\nInvalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                playlist.addSong();
                break;

            case 2:
                playlist.displayPlaylist();
                break;

            case 3:
                playlist.deleteSong();
                break;

            case 4:
                playlist.playSong();
                break;

            case 5:
                playlist.nextSong();
                break;

            case 6:
                playlist.previousSong();
                break;

            case 7:
                playlist.searchSong();
                break;

            case 8:
                playlist.showRecentlyPlayed();
                break;

            case 9:
                cout << "\nExiting Music Playlist Manager...\n";
                break;

            default:
                cout << "\nInvalid choice! Please enter 1-9.\n";
        }

    } while (choice != 9);

    return 0;
}
