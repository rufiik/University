import React, { useState, useEffect } from "react";
import { useRouter } from 'next/router';

const Users = () => {
  const [users, setUsers] = useState([]);
  const [newUser, setNewUser] = useState({
    username: "",
    email: "",
    role: "USER",
    password: "",
  });
  const [editingUser, setEditingUser] = useState(null);
  const [sortConfig, setSortConfig] = useState({ key: null, direction: 'asc' });
  const [filters, setFilters] = useState({
    username: "",
    email: "",
    role: ""
  });
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [username, setUsername] = useState('');
  const router = useRouter();

  useEffect(() => {
    const loggedIn = localStorage.getItem('isLoggedIn');
    const storedUsername = localStorage.getItem('username');
    const storedRole = localStorage.getItem('role');
    if (loggedIn && storedUsername && storedRole) {
      setIsLoggedIn(true);
      setUsername(storedUsername);
      if (storedRole === 'USER') {
        alert('Nie masz uprawnień do przeglądania tej strony.');
        router.push('/');
      }
    } else {
      router.push('/');
    }
    fetchUsers();
  }, []);

  // Pobieranie listy użytkowników
  const fetchUsers = async () => {
    try {
      const response = await fetch('http://localhost:8080/api/users');
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();
      setUsers(data);
    } catch (error) {
      console.error('Błąd podczas pobierania użytkowników:', error);
    }
  };

  // Dodawanie lub edytowanie użytkownika
  const saveUser = async (e) => {
    e.preventDefault();
    try {
      const userToSave = { ...newUser };

      if (editingUser) {
        // Aktualizacja użytkownika
        const response = await fetch(`http://localhost:8080/api/users/${editingUser.id}`, {
          method: "PUT",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(userToSave),
        });
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const updatedUser = await response.json();
        setUsers((prev) =>
          prev.map((user) => (user.id === editingUser.id ? updatedUser : user))
        );
        setEditingUser(null);
      } else {
        // Dodanie nowego użytkownika
        const response = await fetch("http://localhost:8080/api/users", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(userToSave),
        });
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const addedUser = await response.json();
        setUsers((prev) => [...prev, addedUser]);
      }
      setNewUser({ username: "", email: "", role: "USER", password: "" });
    } catch (error) {
      console.error("Błąd podczas zapisywania użytkownika:", error);
    }
  };

  // Usuwanie użytkownika
  const deleteUser = async (userId) => {
    try {
      const response = await fetch(`http://localhost:8080/api/users/${userId}`, {
        method: "DELETE",
      });
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      setUsers((prev) => prev.filter((user) => user.id !== userId));
    } catch (error) {
      console.error("Błąd podczas usuwania użytkownika:", error);
    }
  };

  // Funkcja sortowania
  const sortUsers = (key) => {
    let direction = 'asc';
    if (sortConfig.key === key && sortConfig.direction === 'asc') {
      direction = 'desc';
    }
    setSortConfig({ key, direction });
  };

  const sortedUsers = React.useMemo(() => {
    let sortableUsers = [...users];
    if (sortConfig.key !== null) {
      sortableUsers.sort((a, b) => {
        if (a[sortConfig.key] < b[sortConfig.key]) {
          return sortConfig.direction === 'asc' ? -1 : 1;
        }
        if (a[sortConfig.key] > b[sortConfig.key]) {
          return sortConfig.direction === 'asc' ? 1 : -1;
        }
        return 0;
      });
    }
    return sortableUsers;
  }, [users, sortConfig]);

  // Funkcja filtrowania
  const filteredUsers = sortedUsers.filter((user) => {
    return (
      user.username.toLowerCase().includes(filters.username.toLowerCase()) &&
      user.email.toLowerCase().includes(filters.email.toLowerCase()) &&
      user.role.toLowerCase().includes(filters.role.toLowerCase())
    );
  });

  // Funkcja resetowania filtrów
  const resetFilters = () => {
    setFilters({
      username: "",
      email: "",
      role: ""
    });
  };

  const handleLogout = () => {
    setIsLoggedIn(false);
    setUsername('');
    localStorage.removeItem('isLoggedIn');
    localStorage.removeItem('username');
    localStorage.removeItem('role');
    window.location.href = '/';
  };

  return (
    <div className="flex flex-col items-center justify-center min-h-screen bg-gray-100 p-4">
      <div className="flex justify-center items-center w-full max-w-4xl mb-6">
        <button
          onClick={() => (window.location.href = "/")}
          className="flex items-center bg-green-500 text-white px-4 py-2 rounded hover:bg-green-600 transition">
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
      </div>
      <h1 className="text-3xl font-bold text-blue-600 mb-6">Lista użytkowników</h1>
      <form onSubmit={saveUser} className="mb-6">
        <input
          type="text"
          placeholder="Nazwa użytkownika"
          value={newUser.username}
          onChange={(e) => setNewUser({ ...newUser, username: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="email"
          placeholder="Email"
          value={newUser.email}
          onChange={(e) => setNewUser({ ...newUser, email: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="password"
          placeholder="Hasło"
          value={newUser.password}
          onChange={(e) => setNewUser({ ...newUser, password: e.target.value })}
          className="border p-2 mb-2"
        />
        <select
          value={newUser.role}
          onChange={(e) => setNewUser({ ...newUser, role: e.target.value })}
          className="border p-2 mb-2"
        >
          <option value="USER">USER</option>
          <option value="ADMIN">ADMIN</option>
        </select>
        <button type="submit" className="bg-blue-500 text-white px-4 py-2 rounded">
          {editingUser ? "Zapisz zmiany" : "Dodaj użytkownika"}
        </button>
      </form>
      <div className="mb-4">
        <input
          type="text"
          placeholder="Filtruj nazwę użytkownika"
          value={filters.username}
          onChange={(e) => setFilters({ ...filters, username: e.target.value })}
          className="border p-2 mb-2"
        />
        <input
          type="text"
          placeholder="Filtruj email"
          value={filters.email}
          onChange={(e) => setFilters({ ...filters, email: e.target.value })}
          className="border p-2 mb-2"
        />
        <select
          value={filters.role}
          onChange={(e) => setFilters({ ...filters, role: e.target.value })}
          className="border p-2 mb-2"
        >
          <option value="">Filtruj rolę</option>
          <option value="USER">USER</option>
          <option value="ADMIN">ADMIN</option>
        </select>
        <button onClick={resetFilters} className="bg-gray-500 text-white px-4 py-2 rounded">
          Zresetuj filtry
        </button>
      </div>
      <table className="min-w-full bg-white border border-gray-200">
        <thead>
          <tr>
            <th className="border px-4 py-2">
              Nazwa użytkownika
              <button onClick={() => sortUsers('username')}>
                {sortConfig.key === 'username' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              Email
              <button onClick={() => sortUsers('email')}>
                {sortConfig.key === 'email' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">
              Rola
              <button onClick={() => sortUsers('role')}>
                {sortConfig.key === 'role' && sortConfig.direction === 'asc' ? ' 🔼' : ' 🔽'}
              </button>
            </th>
            <th className="border px-4 py-2">Akcje</th>
          </tr>
        </thead>
        <tbody>
          {filteredUsers.map((user) => (
            <tr key={user.id}>
              <td className="border px-4 py-2">{user.username}</td>
              <td className="border px-4 py-2">{user.email}</td>
              <td className="border px-4 py-2">{user.role}</td>
              <td className="border px-4 py-2">
                <button
                  onClick={() => {
                    setEditingUser(user);
                    setNewUser({
                      username: user.username,
                      email: user.email,
                      role: user.role,
                      password: "",
                    });
                  }}
                  className="bg-yellow-500 text-white px-2 py-1 rounded mr-2 hover:bg-yellow-600 transition"
                >
                  Edytuj
                </button>
                <button
                  onClick={() => deleteUser(user.id)}
                  className="bg-red-500 text-white px-2 py-1 rounded hover:bg-red-600 transition"
                >
                  Usuń
                </button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default Users;