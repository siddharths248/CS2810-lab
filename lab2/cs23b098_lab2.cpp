#include <bits/stdc++.h>
using namespace std;

class Book
{
public:
    string title;
    string author;

    Book(string t, string auth, string is, int cop, int tot)
    {
        title = t;
        author = auth;
        isbn = is;
        copiesAvailable = cop;
        totalCopies = tot;
    }

    Book()
    {
        title = "UnknownTitle";
        author = "UnknownAuthor";
        isbn = "ISBN";
        copiesAvailable = 0;
        totalCopies = 5;
    }

    Book(Book &b, string newisbn)
    {
        title = b.title;
        author = b.author;
        isbn = newisbn;
        copiesAvailable = b.copiesAvailable;
        totalCopies = b.totalCopies;
    }

    string getISBN()
    {
        return isbn;
    }

    int getCopiesAvailable()
    {
        return copiesAvailable;
    }

    int getTotalCopies()
    {
        return totalCopies;
    }

    void updateCopies(int count)
    {
        if (count < 0)
        {
            if (copiesAvailable + count < 0 || totalCopies + count < 0)
            {
                std::cout << "Invalid request! Count becomes negative" << endl;
            }
            else
            {
                copiesAvailable += count;
                totalCopies += count;
            }
        }
        else
        {
            if (totalCopies + count <= 15)
            {
                copiesAvailable += count;
                totalCopies += count;
            }
            else
            {
                return;
            }
        }
    }

    void updateAvailableCopies(int count, int sign)
    {
        if (sign == 0)
        {
            if (copiesAvailable - count < 0)
            {
                return;
            }
            else
            {
                copiesAvailable -= count;
            }
        }
        else
        {
            copiesAvailable += count;
        }
    }

    bool borrowBook()
    {
        if (copiesAvailable == 0)
        {
            cout << "Invalid request! Copy of book not available" << endl;
            return false;
        }
        else
        {
            copiesAvailable--;
            return true;
        }
    }

    bool returnBook()
    {
        if (copiesAvailable == totalCopies)
        {
            cout << "Invalid request! Copy of book exceeds total copies" << endl;
            return false;
        }
        else
        {
            // updateAvailableCopies(1, 1);
            copiesAvailable++;
            return true;
        }
    }

    void printDetails()
    {
        cout << title << " " << author << endl;
    }

private:
    string isbn;
    int copiesAvailable;
    int totalCopies;

    friend class Member;
    friend class Library;
};

class Member
{
public:
    string name;
    int bc;

    Member()
    {
        name = "unitialized";
        memberID = "uninit";
        bc = 0;
    }

    Member(string mid, string n, int bl)
    {
        memberID = mid;
        name = n;
        borrowLimit = bl;
        bc = 0;
    }

    Member(string mid, string n)
    {
        memberID = mid;
        name = n;
        borrowLimit = 3;
        bc = 0;
    }

    bool borrowBook(string is)
    {
        if (bc == borrowLimit)
        {
            std::cout << "Invalid request! Borrow limit exceeded" << endl;
            return false;
        }
        else
        {
            bc++;
            borrowedBooks[is]++;
            return true;
        }
    }

    bool returnBook(string is)
    {
        if (borrowedBooks.find(is) == borrowedBooks.end())
        {
            std::cout << "Invalid request! Book not borrowed" << endl;
            return false;
        }
        else
        {
            if (borrowedBooks[is] == 0)
            {
                std::cout << "Invalid request! Book not borrowed" << endl;
                return false;
            }
            else
            {
                bc--;
                borrowedBooks[is]--;
                return true;
            }
        }
    }

    void printDetails()
    {
        for (pair<string, int> p : borrowedBooks)
        {
            if (p.second != 0)
            {
                cout << memberID << " " << name << " " << p.first << " " << p.second << endl;
            }
        }
    }

private:
    string memberID;
    map<string, int> borrowedBooks;
    int borrowLimit;

    friend class Library;
};

class Library
{
public:
    map<string, Book *> book_map;
    map<string, Member *> member_map;

    bool addBook(Book *b)
    {
        if (book_map.find(b->isbn) != book_map.end())
        {
            std::cout << "Invalid request! Book with same isbn already exists" << endl;
            return false;
        }
        else
        {
            if (book_count < 50)
            {
                book_map[b->isbn] = b;
                books.push_back(b);
                book_count++;
                return true;
            }
            else
            {
                // std::cout << "Invalid request! Maximum number of books reached" << endl;
                return false;
            }
        }
    }

    bool registerMember(Member *m)
    {
        if (member_map.find(m->memberID) != member_map.end())
        {
            std::cout << "Invalid request! Member with same id already exists" << endl;
            return false;
        }
        else
        {
            if (member_count < 150)
            {
                member_map[m->memberID] = m;
                members.push_back(m);
                member_count++;
                return true;
            }
            else
            {
                // cout << "Invalid request! Maximum number of members reached" << endl;
                return false;
            }
        }
    }

    bool borrowBook(string mID, string is)
    {
        if ((book_map.find(is) != book_map.end()) && (member_map.find(mID) != member_map.end()))
        {
            if (book_map[is]->copiesAvailable > 0)
            {
                if (member_map[mID]->bc < member_map[mID]->borrowLimit)
                {
                    member_map[mID]->borrowBook(is);
                    book_map[is]->borrowBook();
                    return true;
                }
                else
                {
                    cout << "Invalid request! Borrow limit exceeded" << endl;
                    return false;
                }
            }
            else
            {
                cout << "Invalid request! Copy of book not available" << endl;
                return false;
            }
        }
        else
        {
            // cout << "Invalid request! Book or Member not in library" << endl;
            return false;
        }
    }

    bool returnBook(string mID, string is)
    {
        if ((book_map.find(is) != book_map.end()) && (member_map.find(mID) != member_map.end()))
        {
            if (book_map[is]->copiesAvailable < book_map[is]->totalCopies)
            {
                if ((member_map[mID]->borrowedBooks.find(is) != member_map[mID]->borrowedBooks.end()) && (member_map[mID]->borrowedBooks[is] > 0))
                {
                    member_map[mID]->returnBook(is);
                    book_map[is]->returnBook();
                    return true;
                }
                else
                {
                    cout << "Invalid request! Book not borrowed" << endl;
                    return false;
                }
            }
            else
            {
                cout << "Invalid request! Copy of book exceeds total copies" << endl;
                return false;
            }
        }
        else
        {
            // cout << "Invalid request! Book or Member not in library" << endl;
            return false;
        }
    }

    void printLibraryDetails()
    {
        for (Book *b : books)
        {
            cout << b->title << " " << b->author << " " << b->copiesAvailable << endl;
        }
        for (Member *m : members)
        {
            cout << m->memberID << " " << m->name << endl;
        }
    }

private:
    vector<Book *> books;
    vector<Member *> members;
    int book_count;
    int member_count;
    friend class Member;
};

int main()
{
    Library library;
    string command;

    while (cin >> command)
    {

        if (command == "Done")
        {
            break;
        }

        else if (command == "Book")
        {
            string title, author, isbn;
            int avl, tot;
            string f;
            cin >> f;

            if (f == "None")
            {
                Book *b = new Book;
                library.addBook(b);
            }
            else if (f == "ExistingBook")
            {
                string old, ne;
                cin >> old >> ne;
                Book *eb = library.book_map[old];
                Book *nb = new Book(*eb, ne);
                library.addBook(nb);
            }
            else
            {
                title = f;
                cin >> author >> isbn >> avl >> tot;
                if (tot <= 15)
                {
                    Book *b = new Book(title, author, isbn, avl, tot);
                    library.addBook(b);
                }
            }
        }

        else if (command == "UpdateCopiesCount")
        {
            string isbn;
            int c;
            cin >> isbn >> c;
            library.book_map[isbn]->updateCopies(c);
        }

        else if (command == "Member")
        {
            string mID, name;
            int bl;
            string f;

            cin >> f;

            if (f == "NoBorrowLimit")
            {
                cin >> mID >> name;
                Member *m = new Member(mID, name);
                library.registerMember(m);
            }
            else
            {
                mID = f;
                cin >> name >> bl;
                if (bl <= 15)
                {
                    Member *m = new Member(mID, name, bl);
                    library.registerMember(m);
                }
            }
        }

        else if (command == "Borrow")
        {
            string mID, isbn;
            cin >> mID >> isbn;
            library.borrowBook(mID, isbn);
        }

        else if (command == "Return")
        {
            string mID, isbn;
            cin >> mID >> isbn;
            library.returnBook(mID, isbn);
        }

        else if (command == "PrintBook")
        {
            string isbn;
            cin >> isbn;
            library.book_map[isbn]->printDetails();
        }

        else if (command == "PrintMember")
        {
            string mid;
            cin >> mid;
            library.member_map[mid]->printDetails();
        }

        else if (command == "PrintLibrary")
        {
            library.printLibraryDetails();
        }
    }

    return 0;
}
