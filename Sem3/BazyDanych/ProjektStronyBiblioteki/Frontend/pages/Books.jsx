import React, { useState, useEffect } from "react";

const Books = () => {
  const [books, setBooks] = useState([]);
  const [errorMessage, setErrorMessage] = useState('');
  const [role, setRole] = useState('');
  const [newBook, setNewBook] = useState({
    title: "",
    author: {
      firstName: "",
      lastName: "",
      dateOfBirth: "",
      nationality: ""
    },
    genre: "",
    isbn: "",
    available: true,
  });
  const [editingBook, setEditingBook] = useState(null);
  const [sortConfig, setSortConfig] = useState({ key: null, direction: 'asc' });
  const [filters, setFilters] = useState({
    title: "",
    authorFirstName: "",
    authorLastName: "",
    authorNationality: "",
    genre: "",
    isbn: "",
    available: ""
  });

  // Pobieranie książek
  const fetchBooks = async () => {
    try {
      const response = await fetch("http://localhost:8080/api/books");
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();
      setBooks(data);
    } catch (error) {
      console.error("Błąd podczas pobierania książek:", error);
    }
  };

  // Dodawanie lub edytowanie książki
  const saveBook = async (e) => {
    e.preventDefault();
    try {
      const bookToSave = { ...newBook };

      if (editingBook) {
        // Aktualizacja książki
        const response = await fetch(`http://localhost:8080/api/books/${editingBook.id}`, {
          method: "PUT",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(bookToSave),
        });
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const updatedBook = await response.json();
        setBooks((prev) =>
          prev.map((book) => (book.id === editingBook.id ? updatedBook : book))
        );
        setEditingBook(null);
      } else {
        // Dodanie nowej książki
        const response = await fetch("http://localhost:8080/api/books", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(bookToSave),
        });
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const addedBook = await response.json();
        setBooks((prev) => [...prev, addedBook]);
      }
      setNewBook({
        title: "",
        author: {
          firstName: "",
          lastName: "",
          dateOfBirth: "",
          nationality: ""
        },
        genre: "",
        isbn: "",
        available: true
      });
    } catch (error) {
      console.error("Błąd podczas zapisywania książki:", error);
    }
  };

  // Usuwanie książki
  const deleteBook = async (bookId) => {
    try {
      console.log(`Usuwanie książki o id: ${bookId}`);
      const response = await fetch(`http://localhost:8080/api/books/${bookId}`, {
        
        method: "DELETE",
      });
      if (!response.ok) {
        if (response.status === 500) {
          throw new Error('Nie można usunąć książki. Może być powiązana z innymi danymi.');
        } else {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
      }
      setBooks((prev) => prev.filter((book) => book.id !== bookId));
    } catch (error) {
      console.error("Błąd podczas usuwania książki:", error);
      setErrorMessage('Nie można usunąć książki. Może być powiązana z innymi danymi.');
    }
  };

  // Funkcja sortowania
  const sortBooks = (key) => {
    let direction = 'asc';
    if (sortConfig.key === key && sortConfig.direction === 'asc') {
      direction = 'desc';
    }
    setSortConfig({ key, direction });
  };

  const sortedBooks = React.useMemo(() => {
    let sortableBooks = [...books];
    if (sortConfig.key !== null) {
      sortableBooks.sort((a, b) => {
        if (a[sortConfig.key] < b[sortConfig.key]) {
          return sortConfig.direction === 'asc' ? -1 : 1;
        }
        if (a[sortConfig.key] > b[sortConfig.key]) {
          return sortConfig.direction === 'asc' ? 1 : -1;
        }
        return 0;
      });
    }
    return sortableBooks;
  }, [books, sortConfig]);

  // Funkcja filtrowania
  const filteredBooks = sortedBooks.filter((book) => {
    return (
      book.title.toLowerCase().includes(filters.title.toLowerCase()) &&
      (book.author && book.author.firstName.toLowerCase().includes(filters.authorFirstName.toLowerCase())) &&
      (book.author && book.author.lastName.toLowerCase().includes(filters.authorLastName.toLowerCase())) &&
      (book.author && book.author.nationality.toLowerCase().includes(filters.authorNationality.toLowerCase())) &&
      (typeof book.genre === 'object' && book.genre.name.toLowerCase().includes(filters.genre.toLowerCase())) &&
      book.isbn.includes(filters.isbn) &&
      (filters.available === "" || (filters.available === "true" && book.available) || (filters.available === "false" && !book.available))
    );
  });

  // Funkcja resetowania filtrów
  const resetFilters = () => {
    setFilters({
      title: "",
      authorFirstName: "",
      authorLastName: "",
      authorNationality: "",
      genre: "",
      isbn: "",
      available: ""
    });
  };
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [username, setUsername] = useState('');
  useEffect(() => {
    const loggedIn = localStorage.getItem('isLoggedIn');
    const storedUsername = localStorage.getItem('username');
    const storedRole = localStorage.getItem('role');
    if (loggedIn && storedUsername) {
      setIsLoggedIn(true);
      setUsername(storedUsername);
      setRole(storedRole);
    }
  }, []);

  const handleLogout = () => {
    setIsLoggedIn(false);
    setUsername('');
    localStorage.removeItem('isLoggedIn');
    localStorage.removeItem('username');
    localStorage.removeItem('role');
    window.location.href = '/';
  };


  useEffect(() => {
    fetchBooks();
  }, []);

  return (
    
    <div className="flex flex-col items-center justify-center min-h-screen bg-gray-100 p-4">
      <button
  onClick={() => (window.location.href = "/")}
  className="bg-green-500 text-white px-4 py-2 rounded mb-6 hover:bg-green-600 transition"
>
  Powrót do strony głównej
</button>
{isLoggedIn && (
  <div className="flex items-center space-x-4 absolute top-4 right-4">
    <span className="text-lg text-gray-700">Zalogowany jako: {username}</span>
    <button
      onClick={handleLogout}
      className="bg-red-500 text-white px-4 py-2 rounded hover:bg-red-600 transition"
    >
      Wyloguj się
    </button>
  </div>
)}
      <h1 className="text-3xl font-bold text-blue-600 mb-6">Lista książek</h1>
      {role !== 'USER' && (
      <form onSubmit={saveBook} className="mb-6 flex flex-wrap items-center">
        <input
          type="text"
          placeholder="Tytuł"
          value={newBook.title}
          onChange={(e) => setNewBook({ ...newBook, title: e.target.value })}
          className="border p-2 mb-2 mr-2"
        />
        <input
          type="text"
          placeholder="Imię autora"
          value={newBook.author.firstName}
          onChange={(e) => setNewBook({ ...newBook, author: { ...newBook.author, firstName: e.target.value } })}
          className="border p-2 mb-2 mr-2"
        />
        <input
          type="text"
          placeholder="Nazwisko autora"
          value={newBook.author.lastName}
          onChange={(e) => setNewBook({ ...newBook, author: { ...newBook.author, lastName: e.target.value } })}
          className="border p-2 mb-2 mr-2"
        />
        <input
          type="date"
          placeholder="Data urodzenia autora"
          value={newBook.author.dateOfBirth}
          onChange={(e) => setNewBook({ ...newBook, author: { ...newBook.author, dateOfBirth: e.target.value } })}
          className="border p-2 mb-2 mr-2"
        />
        <input
          type="text"
          placeholder="Narodowość autora"
          value={newBook.author.nationality}
          onChange={(e) => setNewBook({ ...newBook, author: { ...newBook.author, nationality: e.target.value } })}
          className="border p-2 mb-2 mr-2"
        />
        <input
          type="text"
          placeholder="Gatunek"
          value={newBook.genre}
          onChange={(e) => setNewBook({ ...newBook, genre: e.target.value })}
          className="border p-2 mb-2 mr-2"
        />
        <input
          type="text"
          placeholder="ISBN"
          value={newBook.isbn}
          onChange={(e) => setNewBook({ ...newBook, isbn: e.target.value })}
          className="border p-2 mb-2 mr-2"
        />
        <label className="flex items-center mb-2 mr-2">
          <input
            type="checkbox"
            checked={newBook.available}
            onChange={(e) => setNewBook({ ...newBook, available: e.target.checked })}
            className="mr-2"
          />
          Dostępna
        </label>
        <button type="submit" className="bg-blue-500 text-white px-4 py-2 rounded">
          {editingBook ? "Zapisz zmiany" : "Dodaj książkę"}
        </button>
      </form>
      )}
      <div className="mb-4">
        <input
          type="text"
          placeholder="Filtruj tytuł"
          value={filters.title}
          onChange={(e) => setFilters({ ...filters, title: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="text"
          placeholder="Filtruj imię autora"
          value={filters.authorFirstName}
          onChange={(e) => setFilters({ ...filters, authorFirstName: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="text"
          placeholder="Filtruj nazwisko autora"
          value={filters.authorLastName}
          onChange={(e) => setFilters({ ...filters, authorLastName: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="text"
          placeholder="Filtruj narodowość autora"
          value={filters.authorNationality}
          onChange={(e) => setFilters({ ...filters, authorNationality: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="text"
          placeholder="Filtruj gatunek"
          value={filters.genre}
          onChange={(e) => setFilters({ ...filters, genre: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="text"
          placeholder="Filtruj ISBN"
          value={filters.isbn}
          onChange={(e) => setFilters({ ...filters, isbn: e.target.value })}
          className="border p-2 mb-2"
        />
        <select
          value={filters.available}
          onChange={(e) => setFilters({ ...filters, available: e.target.value })}
          className="border p-2 mb-2"
        >
          <option value="">Filtruj dostępność</option>
          <option value="true">Dostępna</option>
          <option value="false">Niedostępna</option>
        </select>
        <button onClick={resetFilters} className="bg-gray-500 text-white px-4 py-2 rounded">
          Zresetuj filtry
        </button>
      </div>
      <table className="min-w-full bg-white border border-gray-200">
        <thead>
          <tr>
            <th className="border px-4 py-2">
              Tytuł
              <button onClick={() => sortBooks('title')}>
                {sortConfig.key === 'title' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              Autor
              <button onClick={() => sortBooks('author')}>
                {sortConfig.key === 'author' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              Gatunek
              <button onClick={() => sortBooks('genre')}>
                {sortConfig.key === 'genre' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              ISBN
              <button onClick={() => sortBooks('isbn')}>
                {sortConfig.key === 'isbn' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              Dostępna
              <button onClick={() => sortBooks('available')}>
                {sortConfig.key === 'available' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            {role !== 'USER' && <th className="border px-4 py-2">Akcje</th>}
          </tr>
        </thead>
        <tbody>
          {filteredBooks.map((book) => (
            <tr key={book.id}>
              <td className="border px-4 py-2">{book.title}</td>
              <td className="border px-4 py-2">{book.author ? `${book.author.firstName} ${book.author.lastName}` : "Brak autora"}</td>
              <td className="border px-4 py-2">{book.genre ? book.genre.name : "Brak gatunku"}</td>
              <td className="border px-4 py-2">{book.isbn}</td>
              <td className="border px-4 py-2">{book.available ? "Tak" : "Nie"}</td>
              {role !== 'USER' && (
              <td className="border px-4 py-2">
                <button
                  onClick={() => {
                    setEditingBook(book);
                    setNewBook({
                      title: book.title,
                      author: {
                        firstName: book.author ? book.author.firstName : "",
                        lastName: book.author ? book.author.lastName : "",
                        dateOfBirth: book.author ? book.author.dateOfBirth : "",
                        nationality: book.author ? book.author.nationality : ""
                      },
                      genre: book.genre ? book.genre.name : "",
                      isbn: book.isbn,
                      available: book.available,
                    });
                  }}
                  className="bg-yellow-500 text-white px-2 py-1 rounded mr-2 hover:bg-yellow-600 transition"
                >
                  Edytuj
                </button>
                <button
                  onClick={() => deleteBook(book.id)}
                  className="bg-red-500 text-white px-2 py-1 rounded hover:bg-red-600 transition"
                >
                  Usuń
                </button>
              </td>
                )}
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default Books;