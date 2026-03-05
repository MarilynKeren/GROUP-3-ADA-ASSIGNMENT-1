
#include <Arduino.h>
#define MAX_PLAYLIST_SIZE 10

// ==================== TYPE DEFINITIONS ====================
struct Song {
  char title[50];
  char artist[50];
  int duration; // in seconds
};

// ==================== ARRAY-BASED PLAYLIST ====================
struct ArrayList {
  Song songs[MAX_PLAYLIST_SIZE];
  int size = 0;
};

struct SongNode {
  Song song;
  SongNode* next;
};

struct LinkedList {
  SongNode* head = nullptr;
};

// ==================== FUNCTION PROTOTYPES ====================
// Array list functions
void addSong(ArrayList* list, const Song& newSong);
void removeSong(ArrayList* list, int index);
void displayPlaylist(const ArrayList* list);

// Linked list functions
void addSong(LinkedList* list, const Song& newSong);
void removeSong(LinkedList* list, int index);
void displayPlaylist(const LinkedList* list);

// ==================== UTILITY DISPLAY FUNCTIONS ====================

// Prints decorative divider
void printDivider() {
  Serial.println("==================================================");
}

// Simulated progress bar
void playAnimation(const Song& song) {
  Serial.print("Now Playing: ");
  Serial.print(song.title);
  Serial.print(" - ");
  Serial.println(song.artist);

  Serial.print("[");
  for (int i = 0; i < 20; i++) {
    Serial.print("█");
    delay(100);
  }
  Serial.println("] Finished\n");
}

// ==================== ARRAY IMPLEMENTATION ====================

// Adds a song to the end of the array list.
// Checks for overflow before insertion.
void addSong(ArrayList* list, const Song& newSong) {
  if (list->size >= MAX_PLAYLIST_SIZE) {
    Serial.println("⚠ Array Playlist Full!");
    return;
  }

  list->songs[list->size] = newSong;
  list->size++;

  Serial.print("✓ Added (Array): ");
  Serial.println(newSong.title);
}

// Removes a song at the specified index (0-based).
// Shifts remaining elements to maintain continuity.
void removeSong(ArrayList* list, int index) {
  if (index < 0 || index >= list->size) {
    Serial.println("⚠ Invalid index (Array)");
    return;
  }

  Serial.print("✗ Removed (Array): ");
  Serial.println(list->songs[index].title);

  for (int i = index; i < list->size - 1; i++) {
    list->songs[i] = list->songs[i + 1];
  }

  list->size--;
}

// Displays all songs currently stored in the array playlist.
void displayPlaylist(const ArrayList* list) {
  printDivider();
  Serial.println("        ARRAY PLAYLIST");
  printDivider();

  for (int i = 0; i < list->size; i++) {
    Serial.print(i);
    Serial.print(" | ");
    Serial.print(list->songs[i].title);
    Serial.print(" - ");
    Serial.print(list->songs[i].artist);
    Serial.print(" (");
    Serial.print(list->songs[i].duration);
    Serial.println("s)");
  }

  printDivider();
}

// ==================== LINKED LIST IMPLEMENTATION ====================

// Adds a song to the end of the linked list.
// Uses dynamic memory allocation.
void addSong(LinkedList* list, const Song& newSong) {
  SongNode* newNode = new SongNode;
  newNode->song = newSong;
  newNode->next = nullptr;

  if (list->head == nullptr) {
    list->head = newNode;
  } else {
    SongNode* temp = list->head;
    while (temp->next != nullptr) {
      temp = temp->next;
    }
    temp->next = newNode;
  }

  Serial.print("✓ Added (Linked List): ");
  Serial.println(newSong.title);
}

// Removes a song at a specified index in the linked list.
// Properly frees memory to prevent leaks.
void removeSong(LinkedList* list, int index) {
  if (list->head == nullptr) {
    Serial.println("⚠ Linked List Empty");
    return;
  }

  if (index == 0) {
    SongNode* temp = list->head;
    list->head = temp->next;

    Serial.print("✗ Removed (Linked List): ");
    Serial.println(temp->song.title);

    delete temp;
    return;
  }

  SongNode* current = list->head;
  for (int i = 0; current != nullptr && i < index - 1; i++) {
    current = current->next;
  }

  if (current == nullptr || current->next == nullptr) {
    Serial.println("⚠ Invalid index (Linked List)");
    return;
  }

  SongNode* temp = current->next;
  current->next = temp->next;

  Serial.print("✗ Removed (Linked List): ");
  Serial.println(temp->song.title);

  delete temp;
}

// Displays all songs stored in the linked list.
void displayPlaylist(const LinkedList* list) {
  printDivider();
  Serial.println("       LINKED LIST PLAYLIST");
  printDivider();

  SongNode* temp = list->head;
  int index = 0;

  while (temp != nullptr) {
    Serial.print(index++);
    Serial.print(" | ");
    Serial.print(temp->song.title);
    Serial.print(" - ");
    Serial.print(temp->song.artist);
    Serial.print(" (");
    Serial.print(temp->song.duration);
    Serial.println("s)");

    temp = temp->next;
  }

  printDivider();
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Test Array Playlist
  ArrayList myArrayPlaylist;
  addSong(&myArrayPlaylist, {"Song A1", "Artist 1", 180});
  addSong(&myArrayPlaylist, {"Song B2", "Artist 2", 240});
  addSong(&myArrayPlaylist, {"Song C3", "Artist 3", 200});
  displayPlaylist(&myArrayPlaylist);

  playAnimation(myArrayPlaylist.songs[0]);

  removeSong(&myArrayPlaylist, 1);
  displayPlaylist(&myArrayPlaylist);

  // Test Linked List Playlist
  LinkedList myLinkedListPlaylist;
  addSong(&myLinkedListPlaylist, {"Song X1", "Artist X", 195});
  addSong(&myLinkedListPlaylist, {"Song Y2", "Artist Y", 225});
  addSong(&myLinkedListPlaylist, {"Song Z3", "Artist Z", 215});
  displayPlaylist(&myLinkedListPlaylist);

  removeSong(&myLinkedListPlaylist, 1);
  displayPlaylist(&myLinkedListPlaylist);

  // Free memory
  SongNode* current = myLinkedListPlaylist.head;
  while (current != nullptr) {
    SongNode* temp = current;
    current = current->next;
    delete temp;
  }
}

void loop() {}

/*
==================== ANALYSIS ====================

1) ARRAY-BASED PLAYLIST

Add Operation:
- Time Complexity: O(1)
  The song is added at the end using the size index.
- Space Complexity: O(n)
  Memory is preallocated with a fixed maximum size.

Remove Operation:
- Time Complexity: O(n)
  Elements must be shifted left after removal.
- Space Complexity: O(1)
  No additional memory is allocated during removal.

Advantages:
- Contiguous memory allocation.
- No dynamic memory usage.
- No heap fragmentation.
- More predictable and stable for embedded systems.

Disadvantages:
- Fixed maximum size.
- Removing from middle requires shifting elements.


2) LINKED LIST-BASED PLAYLIST

Add Operation:
- Time Complexity: O(n)
  Requires traversal to the end of the list.
- Space Complexity: O(n)
  Memory allocated dynamically for each node.

Remove Operation:
- Time Complexity: O(n)
  Requires traversal to locate the node.
- Space Complexity: O(1)
  Only pointer reassignment is required.

Advantages:
- Dynamic size (no fixed limit).
- No shifting required when removing elements.

Disadvantages:
- Extra memory used for pointer in each node.
- Uses dynamic memory (heap allocation).
- Risk of heap fragmentation over time.
- Risk of memory leaks if delete is not handled properly.
- Less cache-friendly compared to arrays.


3) COMPARISON AND CONCLUSION

- If the playlist rarely changes:
  The array implementation is more memory-efficient and safer
  because it avoids dynamic memory allocation.

- If songs are frequently added/removed from the middle:
  The linked list is more time-efficient because it avoids shifting.

- Risk on ESP32:
  Linked list implementation may cause heap fragmentation
  and instability over long-term embedded use.

Final Conclusion:
For embedded systems like the ESP32, the array-based implementation
is generally safer and more stable, while the linked list provides
greater flexibility at the cost of memory overhead and potential
heap-related issues.
==================================================
*/