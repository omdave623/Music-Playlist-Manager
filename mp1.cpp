#include <iostream>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

struct Song {
    string title, artist, album;
    float duration;
    Song *prev, *next;
};

struct RecentNode {
    string title, artist, album;
    float duration;
    RecentNode* next;
};

class Playlist {
private:
    Song *head = NULL, *tail = NULL, *current = NULL;
    Song *queueFront = NULL, *queueRear = NULL;
    RecentNode* recentTop = NULL;

    Song* findSong(const string& title) {
        for (Song* p = head; p != NULL; p = p->next)
            if (p->title == title) return p;
        return NULL;
    }

    void pushRecent(Song* song) {
        RecentNode* node = new RecentNode{
            song->title, song->artist, song->album, song->duration, recentTop
        };
        recentTop = node;
    }

    void appendSong(const string& title, const string& artist,
                    const string& album, float duration) {
        Song* node = new Song{title, artist, album, duration, tail, NULL};
        if (head == NULL) head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }

    void clearQueue() {
        while (queueFront != NULL) {
            Song* temp = queueFront;
            queueFront = queueFront->next;
            delete temp;
        }
        queueRear = NULL;
    }

public:
    ~Playlist() {
        while (head != NULL) {
            Song* temp = head;
            head = head->next;
            delete temp;
        }
        while (recentTop != NULL) {
            RecentNode* temp = recentTop;
            recentTop = recentTop->next;
            delete temp;
        }
        clearQueue();
    }

    void loadFromFile() {
        ifstream file("playlist.txt");
        if (!file) return;

        string title, artist, album;
        float duration;

        while (getline(file, title)) {
            if (!getline(file, artist) || !getline(file, album) ||
                !(file >> duration)) {
                cout << "\nWarning: Incomplete data in playlist.txt.\n";
                break;
            }
            file.ignore(numeric_limits<streamsize>::max(), '\n');
            if (duration <= 0) {
                cout << "\nWarning: Invalid duration in playlist.txt.\n";
                break;
            }
            appendSong(title, artist, album, duration);
        }

        if (head != NULL) cout << "\nSaved playlist loaded.\n";
    }

    void saveToFile() {
        ofstream file("playlist.txt");
        if (!file) {
            cout << "\nError: Could not save playlist.txt.\n";
            return;
        }

        for (Song* p = head; p != NULL; p = p->next) {
            file << p->title << '\n' << p->artist << '\n'
                 << p->album << '\n' << p->duration << '\n';
        }
        cout << "\nPlaylist saved.\n";
    }

    void addSong() {
        string title, artist, album;
        float duration;

        cout << "\nSong title: ";
        getline(cin >> ws, title);
        cout << "Artist: ";
        getline(cin, artist);
        cout << "Album: ";
        getline(cin, album);
        cout << "Duration in minutes: ";

        if (!(cin >> duration) || duration <= 0) {
            cout << "Invalid duration.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        appendSong(title, artist, album, duration);
        cout << "Song added.\n";
        saveToFile();
    }

    void displayPlaylist() {
        if (head == NULL) {
            cout << "\nPlaylist is empty.\n";
            return;
        }

        int i = 1;
        cout << "\n===== MY PLAYLIST =====\n";
        for (Song* p = head; p != NULL; p = p->next) {
            cout << "\n" << i++ << ". " << p->title
                 << "\nArtist: " << p->artist
                 << "\nAlbum: " << p->album
                 << "\nDuration: " << p->duration << " minutes";
            if (p == current) cout << "\nStatus: Current Song";
            cout << '\n';
        }
    }

    void deleteSong() {
        if (head == NULL) {
            cout << "\nPlaylist is empty.\n";
            return;
        }

        string title;
        cout << "\nTitle to delete: ";
        getline(cin >> ws, title);
        Song* p = findSong(title);

        if (p == NULL) {
            cout << "Song not found.\n";
            return;
        }

        if (current == p) current = (p->next != NULL) ? p->next : p->prev;
        if (p->prev != NULL) p->prev->next = p->next;
        else head = p->next;
        if (p->next != NULL) p->next->prev = p->prev;
        else tail = p->prev;

        delete p;
        cout << "Song deleted.\n";
        saveToFile();
    }

    void playSong() {
        if (head == NULL) {
            cout << "\nPlaylist is empty.\n";
            return;
        }

        string title;
        cout << "\nTitle to play: ";
        getline(cin >> ws, title);
        current = findSong(title);

        if (current == NULL) {
            cout << "Song not found.\n";
            return;
        }

        pushRecent(current);
        cout << "\nNow playing: " << current->title
             << "\nArtist: " << current->artist
             << "\nAlbum: " << current->album << '\n';
    }

    void nextSong() {
        if (head == NULL) {
            cout << "\nPlaylist is empty.\n";
            return;
        }
        if (current == NULL) current = head;
        else if (current->next != NULL) current = current->next;
        else {
            cout << "\nAlready at the last song.\n";
            return;
        }
        pushRecent(current);
        cout << "\nNow playing: " << current->title << '\n';
    }

    void previousSong() {
        if (head == NULL) {
            cout << "\nPlaylist is empty.\n";
            return;
        }
        if (current == NULL) current = head;
        else if (current->prev != NULL) current = current->prev;
        else {
            cout << "\nAlready at the first song.\n";
            return;
        }
        pushRecent(current);
        cout << "\nNow playing: " << current->title << '\n';
    }

    void searchSong() {
        string title;
        cout << "\nTitle to search: ";
        getline(cin >> ws, title);
        Song* p = findSong(title);

        if (p == NULL) {
            cout << "Song not found.\n";
            return;
        }
        cout << "\nSong found: " << p->title
             << "\nArtist: " << p->artist
             << "\nAlbum: " << p->album
             << "\nDuration: " << p->duration << " minutes\n";
    }

    void showRecentlyPlayed() {
        if (recentTop == NULL) {
            cout << "\nNo recently played songs.\n";
            return;
        }

        cout << "\n===== RECENTLY PLAYED (STACK) =====\n";
        int i = 1;
        for (RecentNode* p = recentTop; p != NULL; p = p->next)
            cout << i++ << ". " << p->title << " - " << p->artist << '\n';
    }

    void addToQueue() {
        if (head == NULL) {
            cout << "\nPlaylist is empty. Add songs first.\n";
            return;
        }

        string title;
        cout << "\nTitle to queue: ";
        getline(cin >> ws, title);
        Song* source = findSong(title);

        if (source == NULL) {
            cout << "Song not found in playlist.\n";
            return;
        }

        Song* node = new Song{
            source->title, source->artist, source->album, source->duration,
            queueRear, NULL
        };

        if (queueRear == NULL) queueFront = queueRear = node;
        else {
            queueRear->next = node;
            queueRear = node;
        }
        cout << "Song added to queue.\n";
    }

    void showQueue() {
        if (queueFront == NULL) {
            cout << "\nUpcoming queue is empty.\n";
            return;
        }

        cout << "\n===== UPCOMING QUEUE (FIFO) =====\n";
        int i = 1;
        for (Song* p = queueFront; p != NULL; p = p->next)
            cout << i++ << ". " << p->title << " - " << p->artist << '\n';
    }

    void playNextFromQueue() {
        if (queueFront == NULL) {
            cout << "\nUpcoming queue is empty.\n";
            return;
        }

        Song* queued = queueFront;
        queueFront = queueFront->next;
        if (queueFront == NULL) queueRear = NULL;
        else queueFront->prev = NULL;

        Song* source = findSong(queued->title);
        if (source == NULL) {
            cout << "\nQueued song is no longer in the playlist: "
                 << queued->title << '\n';
            delete queued;
            return;
        }

        current = source;
        pushRecent(current);
        cout << "\nNow playing from queue: " << current->title
             << "\nArtist: " << current->artist << '\n';
        delete queued;
    }
};

int main() {
    Playlist playlist;
    playlist.loadFromFile();
    int choice;

    do {
        cout << "\n===== MUSIC PLAYLIST MANAGER =====\n"
             << "1. Add Song\n"
             << "2. Display Playlist\n"
             << "3. Delete Song\n"
             << "4. Play Song\n"
             << "5. Next Song\n"
             << "6. Previous Song\n"
             << "7. Search Song\n"
             << "8. Show Recently Played\n"
             << "9. Add Song to Queue\n"
             << "10. Show Upcoming Queue\n"
             << "11. Play Next Song from Queue\n"
             << "12. Save Playlist\n"
             << "13. Exit\n"
             << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "\nInvalid input. Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: playlist.addSong(); break;
            case 2: playlist.displayPlaylist(); break;
            case 3: playlist.deleteSong(); break;
            case 4: playlist.playSong(); break;
            case 5: playlist.nextSong(); break;
            case 6: playlist.previousSong(); break;
            case 7: playlist.searchSong(); break;
            case 8: playlist.showRecentlyPlayed(); break;
            case 9: playlist.addToQueue(); break;
            case 10: playlist.showQueue(); break;
            case 11: playlist.playNextFromQueue(); break;
            case 12: playlist.saveToFile(); break;
            case 13:
                playlist.saveToFile();
                cout << "\nExiting Music Playlist Manager...\n";
                break;
            default:
                cout << "\nInvalid choice. Enter 1-13.\n";
        }
    } while (choice != 13);

    return 0;
}
