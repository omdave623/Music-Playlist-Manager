#include <iostream>
#include <string>
using namespace std;

struct Song {
    string title;
    string artist;
    string album;
    float duration;

    Song* prev;
    Song* next;
};

class Playlist {
private:
    Song* head;
    Song* tail;
    Song* current;
    Song* recentTop;

public:
    Playlist() {
    head = NULL;
    tail = NULL;
    current = NULL;
    recentTop = NULL;
}
    void addSong() {
    string title, artist, album;
    float duration;

    cout << "\nEnter song title: ";
    getline(cin >> ws, title);

    cout << "Enter artist name: ";
    getline(cin, artist);

    cout << "Enter album name: ";
    getline(cin, album);

    cout << "Enter duration (minutes): ";
    cin >> duration;

    Song* newSong = new Song;

    newSong->title = title;
    newSong->artist = artist;
    newSong->album = album;
    newSong->duration = duration;

    newSong->prev = NULL;
    newSong->next = NULL;

    if (head == NULL) {
        head = newSong;
        tail = newSong;
    }
    else {
        tail->next = newSong;
        newSong->prev = tail;
        tail = newSong;
    }

    cout << "\nSong added successfully!\n";
}
void deleteSong() {
    if (head == NULL) {
        cout << "\nPlaylist is empty!\n";
        return;
    }

    string title;
    cout << "\nEnter song title to delete: ";
    getline(cin >> ws, title);

    Song* temp = head;

    // Search for the song
    while (temp != NULL && temp->title != title) {
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "\nSong not found!\n";
        return;
    }

    // If deleting the first song
    if (temp == head) {
        head = temp->next;

        if (head != NULL) {
            head->prev = NULL;
        } else {
            tail = NULL;
        }
    }

    // If deleting the last song
    else if (temp == tail) {
        tail = temp->prev;
        tail->next = NULL;
    }

    // If deleting a middle song
    else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    delete temp;

    cout << "\nSong deleted successfully!\n";
}
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
            cout << "\n===== SONG FOUND =====\n";
            cout << "Title    : " << temp->title << endl;
            cout << "Artist   : " << temp->artist << endl;
            cout << "Album    : " << temp->album << endl;
            cout << "Duration : " << temp->duration << " minutes" << endl;
            cout << "======================\n";
            return;
        }

        temp = temp->next;
    }

    cout << "\nSong not found!\n";
}

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
    cout << "Album: " << current->album << endl;
}
void nextSong() {
    if (current == NULL) {
        if (head == NULL) {
            cout << "\nPlaylist is empty!\n";
            return;
        }

        current = head;
    }
    else if (current->next != NULL) {
        current = current->next;
    }
    else {
        cout << "\nYou are already at the last song!\n";
        return;
    }

    cout << "\nNow Playing: " << current->title << endl;
}

void previousSong() {
    if (current == NULL) {
        if (head == NULL) {
            cout << "\nPlaylist is empty!\n";
            return;
        }

        current = head;
    }
    else if (current->prev != NULL) {
        current = current->prev;
    }
    else {
        cout << "\nYou are already at the first song!\n";
        return;
    }

    cout << "\nNow Playing: " << current->title << endl;
}

    void displayPlaylist() const {
        if (head == NULL) {
            cout << "Playlist is empty.\n";
            return;
        }

        Song* temp = head;

        cout << "\n===== MY PLAYLIST =====\n";

        while (temp != NULL) {
            cout << "Title    : " << temp->title << endl;
            cout << "Artist   : " << temp->artist << endl;
            cout << "Album    : " << temp->album << endl;
            cout << "Duration : " << temp->duration << " min" << endl;
            cout << "-----------------------\n";

            temp = temp->next;
        }
    }

    void pushRecent(Song* song) {
    Song* newNode = new Song;

    newNode->title = song->title;
    newNode->artist = song->artist;
    newNode->album = song->album;
    newNode->duration = song->duration;

    newNode->prev = NULL;
    newNode->next = recentTop;

    recentTop = newNode;
}

void showRecentlyPlayed() {
    if (recentTop == NULL) {
        cout << "\nNo recently played songs.\n";
        return;
    }

    Song* temp = recentTop;

    cout << "\n===== RECENTLY PLAYED =====\n";

    while (temp != NULL) {
        cout << "Title  : " << temp->title << endl;
        cout << "Artist : " << temp->artist << endl;
        cout << "---------------------------\n";

        temp = temp->next;
    }
}

};

int main() {
    Playlist playlist;
    int choice;

    do {
        cout << "\n=================================\n";
        cout << "      MUSIC PLAYLIST MANAGER\n";
        cout << "=================================\n";
        cout << "1. Add Song\n";
        cout << "2. Display Playlist\n";
        cout << "3. Delete Song\n";
        cout << "4. Play Song\n";
        cout << "5. Next Song\n";
        cout << "6. Previous Song\n";
        cout << "7. Search Song\n";
        cout << "8. Show Recently Played\n";
        cout << "9. Exit\n";
        cout << "=================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

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
                cout << "\nExiting the program. Goodbye!\n";

            default:
                cout << "\nInvalid choice! Please enter 1-9.\n";
        }

    } while (choice != 9);

    return 0;
}