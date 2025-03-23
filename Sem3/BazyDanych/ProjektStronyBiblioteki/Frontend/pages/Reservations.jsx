import React, { useState, useEffect } from "react";

const Reservations = () => {
  const [reservations, setReservations] = useState([]);
  const [newReservation, setNewReservation] = useState({
    userId: "",
    bookId: "",
    reservationDate: "",
    dueDate: "",
    status: "zarezerwowana",
  });
  const [editingReservation, setEditingReservation] = useState(null);
  const [users, setUsers] = useState([]);
  const [books, setBooks] = useState([]);
  const [sortConfig, setSortConfig] = useState({ key: null, direction: 'asc' });
  const [filters, setFilters] = useState({
    user: "",
    book: "",
    reservationDate: "",
    dueDate: "",
    status: ""
  });

  // Pobieranie rezerwacji
  const fetchReservations = async () => {
    try {
      const response = await fetch("http://localhost:8080/api/reservations");
      const data = await response.json();
      setReservations(data);
    } catch (error) {
      console.error("Błąd podczas pobierania rezerwacji:", error);
    }
  };

  // Pobieranie użytkowników
  const fetchUsers = async () => {
    try {
      const response = await fetch("http://localhost:8080/api/users");
      const data = await response.json();
      setUsers(data);
    } catch (error) {
      console.error("Błąd podczas pobierania użytkowników:", error);
    }
  };

  // Pobieranie książek
  const fetchBooks = async () => {
    try {
      const response = await fetch("http://localhost:8080/api/books");
      const data = await response.json();
      setBooks(data);
    } catch (error) {
      console.error("Błąd podczas pobierania książek:", error);
    }
  };

  // Dodawanie lub edytowanie rezerwacji
  const saveReservation = async (e) => {
    e.preventDefault();
    try {
      const reservationToSave = {
        user: { id: newReservation.userId },
        book: { id: newReservation.bookId },
        reservationDate: newReservation.reservationDate,
        dueDate: newReservation.dueDate,
        status: newReservation.status,
      };
      if (role === 'USER') {
        const loggedInUser = users.find(user => user.username === username);
        reservationToSave.user.id = loggedInUser.id;
      }
      if (editingReservation) {
        // Aktualizacja rezerwacji
        const response = await fetch(`http://localhost:8080/api/reservations/${editingReservation.id}`, {
          method: "PUT",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(reservationToSave),
        });
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const updatedReservation = await response.json();
        setReservations((prev) =>
          prev.map((reservation) => (reservation.id === editingReservation.id ? updatedReservation : reservation))
        );
        setEditingReservation(null);
      } else {
        // Dodanie nowej rezerwacji
        const response = await fetch("http://localhost:8080/api/reservations", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(reservationToSave),
        });
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const addedReservation = await response.json();
        setReservations((prev) => [...prev, addedReservation]);
        // Zaktualizuj stan użytkowników i książek po dodaniu nowej rezerwacji
        fetchUsers();
        fetchBooks();
      }
      setNewReservation({ userId: "", bookId: "", reservationDate: "", dueDate: "", status: "zarezerwowana" });
    } catch (error) {
      console.error("Błąd podczas zapisywania rezerwacji:", error);
    }
  };

  // Usuwanie rezerwacji
  const deleteReservation = async (reservationId) => {
    try {
      const response = await fetch(`http://localhost:8080/api/reservations/${reservationId}`, {
        method: "DELETE",
      });
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      setReservations((prev) => prev.filter((reservation) => reservation.id !== reservationId));
    } catch (error) {
      console.error("Błąd podczas usuwania rezerwacji:", error);
    }
  };

  // Funkcja sortowania
  const sortReservations = (key) => {
    let direction = 'asc';
    if (sortConfig.key === key && sortConfig.direction === 'asc') {
      direction = 'desc';
    }
    setSortConfig({ key, direction });
  };

  const sortedReservations = React.useMemo(() => {
    let sortableReservations = [...reservations];
    if (sortConfig.key !== null) {
      sortableReservations.sort((a, b) => {
        if (a[sortConfig.key] < b[sortConfig.key]) {
          return sortConfig.direction === 'asc' ? -1 : 1;
        }
        if (a[sortConfig.key] > b[sortConfig.key]) {
          return sortConfig.direction === 'asc' ? 1 : -1;
        }
        return 0;
      });
    }
    return sortableReservations;
  }, [reservations, sortConfig]);

  // Funkcja filtrowania
  const filteredReservations = sortedReservations.filter((reservation) => {
    return (
      (reservation.user?.username?.toLowerCase().includes(filters.user.toLowerCase()) || false) &&
      (reservation.book?.title?.toLowerCase().includes(filters.book.toLowerCase()) || false) &&
      reservation.reservationDate.includes(filters.reservationDate) &&
      reservation.dueDate.includes(filters.dueDate) &&
      reservation.status.toLowerCase().includes(filters.status.toLowerCase())
    );
  });

  // Ustawienie terminu zwrotu na miesiąc później
  const handleReservationDateChange = (e) => {
    const reservationDate = e.target.value;
    const dueDate = new Date(reservationDate);
    dueDate.setMonth(dueDate.getMonth() + 1);
    setNewReservation({ ...newReservation, reservationDate, dueDate: dueDate.toISOString().split("T")[0] });
  };

  // Funkcja resetowania filtrów
  const resetFilters = () => {
    setFilters({
      user: "",
      book: "",
      reservationDate: "",
      dueDate: "",
      status: ""
    });
  };
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [username, setUsername] = useState('');
  const [role, setRole] = useState('');
  useEffect(() => {
    const loggedIn = localStorage.getItem('isLoggedIn');
    const storedUsername = localStorage.getItem('username');
    const storedRole = localStorage.getItem('role');
    if (loggedIn && storedUsername && storedRole) {
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
    fetchReservations();
    fetchUsers();
    fetchBooks();
  }, []);

  return (
    <div className="flex flex-col items-center justify-center min-h-screen bg-gray-100 p-4">
            <button
  onClick={() => (window.location.href = "/")}
  className="bg-green-500 text-white px-4 py-2 rounded mb-6 hover:bg-green-600 transition">
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
      <h1 className="text-3xl font-bold text-blue-600 mb-6">Rezerwacje</h1>
      <form onSubmit={saveReservation} className="mb-6">
      {role !== 'USER' ? (
    <select
      value={newReservation.userId}
      onChange={(e) => setNewReservation({ ...newReservation, userId: e.target.value })}
      className="border p-2 mb-2"
    >
      {users.map((user) => (
        <option key={user.id} value={user.id}>
          {user.username}
        </option>
      ))}
    </select>
  ) : (
    <div className="border p-2 mb-2 bg-gray-200">
      {username}
    </div>
  )}
        <select
          value={newReservation.bookId}
          onChange={(e) => setNewReservation({ ...newReservation, bookId: e.target.value })}
          className="border p-2 mb-2"
        >
          <option value="">Wybierz książkę</option>
          {books.filter(book => book.available).map((book) => (
            <option key={book.id} value={book.id}>
              {book.title}
            </option>
          ))}
        </select>
        <input
          type="date"
          placeholder="Data rezerwacji"
          value={newReservation.reservationDate}
          onChange={handleReservationDateChange}
          className="border p-2 mb-2"
        />
        <input
          type="date"
          placeholder="Termin zwrotu"
          value={newReservation.dueDate}
          readOnly
          className="border p-2 mb-2"
        />
        <button type="submit" className="bg-blue-500 text-white px-4 py-2 rounded">
          {editingReservation ? "Zapisz zmiany" : "Dodaj rezerwację"}
        </button>
      </form>
      <div className="mb-4">
        <input
          type="text"
          placeholder="Filtruj użytkownika"
          value={filters.user}
          onChange={(e) => setFilters({ ...filters, user: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="text"
          placeholder="Filtruj książkę"
          value={filters.book}
          onChange={(e) => setFilters({ ...filters, book: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="date"
          placeholder="Filtruj datę rezerwacji"
          value={filters.reservationDate}
          onChange={(e) => setFilters({ ...filters, reservationDate: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="date"
          placeholder="Filtruj termin zwrotu"
          value={filters.dueDate}
          onChange={(e) => setFilters({ ...filters, dueDate: e.target.value })}
          className="border p-2 mb-2"
        />
        <button onClick={resetFilters} className="bg-gray-500 text-white px-4 py-2 rounded">
          Zresetuj filtry
        </button>
      </div>
      <table className="min-w-full bg-white border border-gray-200">
        <thead>
          <tr>
            <th className="border px-4 py-2">
              Użytkownik
              <button onClick={() => sortReservations('user.username')}>
                {sortConfig.key === 'user.username' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              Książka
              <button onClick={() => sortReservations('book.title')}>
                {sortConfig.key === 'book.title' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              Data rezerwacji
              <button onClick={() => sortReservations('reservationDate')}>
                {sortConfig.key === 'reservationDate' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              Termin zwrotu
              <button onClick={() => sortReservations('dueDate')}>
                {sortConfig.key === 'dueDate' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              Status
              <button onClick={() => sortReservations('status')}>
                {sortConfig.key === 'status' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">Akcje</th>
          </tr>
        </thead>
        <tbody>
  

{filteredReservations.map((reservation) => (
  <tr key={reservation.id}>
    <td className="border px-4 py-2">{reservation.user?.username || 'Brak danych'}</td>
    <td className="border px-4 py-2">{reservation.book?.title || 'Brak danych'}</td>
    <td className="border px-4 py-2">{reservation.reservationDate}</td>
    <td className="border px-4 py-2">{reservation.dueDate}</td>
    <td className="border px-4 py-2">{reservation.status}</td>
    {(role !== 'USER' || reservation.user?.username === username) && (
      <td className="border px-4 py-2">
        <div>
          <button
            onClick={() => {
              setEditingReservation(reservation);
              setNewReservation({
                userId: reservation.user?.id || '',
                bookId: reservation.book?.id || '',
                reservationDate: reservation.reservationDate,
                dueDate: reservation.dueDate,
              });
            }}
            className="bg-yellow-500 text-white px-2 py-1 rounded mr-2 hover:bg-yellow-600 transition">
            Edytuj
          </button>
          <button
            onClick={() => deleteReservation(reservation.id)}
            className="bg-red-500 text-white px-2 py-1 rounded hover:bg-red-600 transition">
            Usuń
          </button>
        </div>
      </td>
    )}
  </tr>
))}
        </tbody>
      </table>
    </div>
  );
};

export default Reservations;
