import React, { useEffect, useState } from 'react';

function Users({ setError }) {
  const [users, setUsers] = useState([]);
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [role, setRole] = useState('user');
  const [editId, setEditId] = useState(null);
  const [editUsername, setEditUsername] = useState('');
  const [editPassword, setEditPassword] = useState('');
  const [editRole, setEditRole] = useState('user');
  const [currentUserId, setCurrentUserId] = useState(null);

  useEffect(() => {
    const token = localStorage.getItem('token');
    if (token) {
      try {
        const payload = JSON.parse(atob(token.split('.')[1]));
        setCurrentUserId(payload.id);
      } catch (e) {
        setCurrentUserId(null);
      }
    }
  }, []);

  const fetchUsers = async () => {
    try {
      const token = localStorage.getItem('token');
      const response = await fetch('http://localhost:3001/api/users', {
        headers: { 'Authorization': `Bearer ${token}` }
      });
      if (!response.ok) throw new Error('Błąd pobierania użytkowników');
      const data = await response.json();
      setUsers(data);
    } catch (err) {
      setError && setError(err.message);
    }
  };

  useEffect(() => {
    fetchUsers();
    // eslint-disable-next-line
  }, []);

  // Dodawanie użytkownika
  const handleAddUser = async (e) => {
    e.preventDefault();
    try {
      const token = localStorage.getItem('token');
      const response = await fetch('http://localhost:3001/api/users', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${token}`
        },
        body: JSON.stringify({ username, password, role }),
      });
      if (!response.ok) throw new Error('Błąd dodawania użytkownika');
      setUsername('');
      setPassword('');
      setRole('user');
      fetchUsers();
    } catch (err) {
      setError && setError(err.message);
    }
  };

  // Usuwanie użytkownika
  const handleDeleteUser = async (id) => {
    if (!window.confirm('Na pewno usunąć tego użytkownika?')) return;
    try {
      const token = localStorage.getItem('token');
      const response = await fetch(`http://localhost:3001/api/users/${id}`, {
        method: 'DELETE',
        headers: { 'Authorization': `Bearer ${token}` }
      });
      if (!response.ok) throw new Error('Błąd usuwania użytkownika');
      fetchUsers();
    } catch (err) {
      setError && setError(err.message);
    }
  };

  // Rozpocznij edycję użytkownika
  const startEdit = (user) => {
    setEditId(user.id || user._id);
    setEditUsername(user.username);
    setEditPassword('');
    setEditRole(user.role || 'user');
  };

  // Zapisz edytowanego użytkownika
  const handleEditUser = async (e) => {
    e.preventDefault();
    try {
      const token = localStorage.getItem('token');
      const response = await fetch(`http://localhost:3001/api/users/${editId}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${token}`
        },
        body: JSON.stringify({ username: editUsername, password: editPassword, role: editRole }),
      });
      if (!response.ok) throw new Error('Błąd edycji użytkownika');
      setEditId(null);
      setEditUsername('');
      setEditPassword('');
      setEditRole('user');
      fetchUsers();
    } catch (err) {
      setError && setError(err.message);
    }
  };

  return (
     <div className="main-container">
      <h2>Użytkownicy</h2>
      <form onSubmit={handleAddUser}>
        <input
          type="text"
          placeholder="Nazwa użytkownika"
          value={username}
          onChange={e => setUsername(e.target.value)}
          required
        />
        <input
          type="password"
          placeholder="Hasło"
          value={password}
          onChange={e => setPassword(e.target.value)}
          required
        />
        <select value={role} onChange={e => setRole(e.target.value)}>
          <option value="user">user</option>
          <option value="admin">admin</option>
        </select>
        <button type="submit">Dodaj użytkownika</button>
      </form>
      <ul>
        {users
          .filter(user => (user.id || user._id) !== currentUserId)
          .map(user => (
          <li key={user.id || user._id}>
            {editId === (user.id || user._id) ? (
              <form onSubmit={handleEditUser} style={{ display: 'inline' }}>
                <input
                  type="text"
                  value={editUsername}
                  onChange={e => setEditUsername(e.target.value)}
                  required
                />
                <input
                  type="password"
                  value={editPassword}
                  placeholder="Nowe hasło"
                  onChange={e => setEditPassword(e.target.value)}
                  required
                />
                <select value={editRole} onChange={e => setEditRole(e.target.value)}>
                  <option value="user">user</option>
                  <option value="admin">admin</option>
                </select>
                <button type="submit">Zapisz</button>
                <button type="button" onClick={() => setEditId(null)}>Anuluj</button>
              </form>
            ) : (
              <>
                <b>{user.username}</b> ({user.role})
                <button onClick={() => startEdit(user)} >Edytuj</button>
                <button onClick={() => handleDeleteUser(user.id || user._id)} >Usuń</button>
              </>
            )}
          </li>
        ))}
      </ul>
    </div>
  );
}

export default Users;