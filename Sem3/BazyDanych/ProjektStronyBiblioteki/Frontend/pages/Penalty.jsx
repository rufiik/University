import React, { useState, useEffect } from "react";
import { useRouter } from 'next/router';
const Penalty = () => {
  const [penalties, setPenalties] = useState([]);
  const [newPenalty, setNewPenalty] = useState({
    userId: "",
    bookId: "",
    penaltyAmount: 0,
    reason: "",
  });
  const [editingPenalty, setEditingPenalty] = useState(null);
  const [users, setUsers] = useState([]);
  const [books, setBooks] = useState([]);

  // Pobieranie kar
  const fetchPenalties = async () => {
    try {
      const response = await fetch("http://localhost:8080/api/penalties");
      const data = await response.json();
      setPenalties(data);
    } catch (error) {
      console.error("Błąd podczas pobierania kar:", error);
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

  // Dodawanie lub edytowanie kary
  const savePenalty = async (e) => {
    e.preventDefault();
    try {
      const penaltyToSave = {
        user: { id: newPenalty.userId },
        book: { id: newPenalty.bookId },
        penaltyAmount: newPenalty.penaltyAmount,
        reason: newPenalty.reason,
      };

      if (editingPenalty) {
        // Aktualizacja kary
        const response = await fetch(`http://localhost:8080/api/penalties/${editingPenalty.id}`, {
          method: "PUT",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(penaltyToSave),
        });
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const updatedPenalty = await response.json();
        setPenalties((prev) =>
          prev.map((penalty) => (penalty.id === editingPenalty.id ? updatedPenalty : penalty))
        );
        setEditingPenalty(null);
      } else {
        // Dodanie nowej kary
        const response = await fetch("http://localhost:8080/api/penalties", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(penaltyToSave),
        });
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const addedPenalty = await response.json();
        setPenalties((prev) => [...prev, addedPenalty]);
      }
      setNewPenalty({ userId: "", bookId: "", penaltyAmount: 0, reason: "" });
    } catch (error) {
      console.error("Błąd podczas zapisywania kary:", error);
    }
  };

  // Usuwanie kary
  const deletePenalty = async (penaltyId) => {
    try {
      const response = await fetch(`http://localhost:8080/api/penalties/${penaltyId}`, {
        method: "DELETE",
      });
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      setPenalties((prev) => prev.filter((penalty) => penalty.id !== penaltyId));
    } catch (error) {
      console.error("Błąd podczas usuwania kary:", error);
    }
  };
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [username, setUsername] = useState('');
  const [role, setRole] = useState('');
  const router = useRouter();

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
    setRole('');
    localStorage.removeItem('isLoggedIn');
    localStorage.removeItem('username');
    localStorage.removeItem('role');
    window.location.href = '/';
  };

  const filteredPenalties = role === 'USER'
    ? penalties.filter(penalty => penalty.user.username === username)
    : penalties;
  useEffect(() => {
    fetchPenalties();
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
      <h1 className="text-3xl font-bold text-blue-600 mb-6">Kary</h1>
      {role !== 'USER' && (
      <form onSubmit={savePenalty} className="mb-6">
        <select
          value={newPenalty.userId}
          onChange={(e) => setNewPenalty({ ...newPenalty, userId: e.target.value })}
          className="border p-2 mb-2"
        >
          <option value="">Wybierz użytkownika</option>
          {users.map((user) => (
            <option key={user.id} value={user.id}>
              {user.username}
            </option>
          ))}
        </select>
        <select
          value={newPenalty.bookId}
          onChange={(e) => setNewPenalty({ ...newPenalty, bookId: e.target.value })}
          className="border p-2 mb-2"
        >
          <option value="">Wybierz książkę</option>
          {books.map((book) => (
            <option key={book.id} value={book.id}>
              {book.title}
            </option>
          ))}
        </select>
        <input
          type="number"
          placeholder="Kwota kary"
          value={newPenalty.penaltyAmount}
          onChange={(e) => setNewPenalty({ ...newPenalty, penaltyAmount: parseFloat(e.target.value) })}
          className="border p-2 mb-2"
        />
        <input
          type="text"
          placeholder="Powód"
          value={newPenalty.reason}
          onChange={(e) => setNewPenalty({ ...newPenalty, reason: e.target.value })}
          className="border p-2 mb-2"
        />
        <button type="submit" className="bg-blue-500 text-white px-4 py-2 rounded">
          {editingPenalty ? "Zapisz zmiany" : "Dodaj karę"}
        </button>
      </form>
            )}
      <table className="min-w-full bg-white border border-gray-200">
        <thead>
          <tr>
            <th className="border px-4 py-2">Użytkownik</th>
            <th className="border px-4 py-2">Książka</th>
            <th className="border px-4 py-2">Kwota kary</th>
            <th className="border px-4 py-2">Powód</th>
            {role !== 'USER' && <th className="border px-4 py-2">Akcje</th>}
          </tr>
        </thead>
        <tbody>
  {filteredPenalties.map((penalty) => (
    <tr key={penalty.id}>
      <td className="border px-4 py-2">{penalty.user.username}</td>
      <td className="border px-4 py-2">{penalty.book.title}</td>
      <td className="border px-4 py-2">{penalty.penaltyAmount}</td>
      <td className="border px-4 py-2">{penalty.reason}</td>
      {role !== 'USER' && (
        <td className="border px-4 py-2">
          <div>
            <button
              onClick={() => {
                setEditingPenalty(penalty);
                setNewPenalty({
                  userId: penalty.user.id,
                  bookId: penalty.book.id,
                  penaltyAmount: penalty.penaltyAmount,
                  reason: penalty.reason,
                });
              }}
              className="bg-yellow-500 text-white px-2 py-1 rounded mr-2 hover:bg-yellow-600 transition">
              Edytuj
            </button>
            <button
              onClick={() => deletePenalty(penalty.id)}
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

export default Penalty;