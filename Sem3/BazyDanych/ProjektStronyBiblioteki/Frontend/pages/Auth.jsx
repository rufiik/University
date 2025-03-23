// import React, { useState } from 'react';
// import Login from './Login';

// const Auth = () => {
//   const [role, setRole] = useState('');
//   const [error, setError] = useState('');

//   const handleLogin = async (username, password) => {
//     try {
//       const response = await fetch("http://localhost:8080/api/auth/role", {
//         headers: {
//           'Authorization': 'Basic ' + btoa(`${username}:${password}`),
//           'Content-Type': 'application/json'
//         },
//         credentials: 'include'
//       });
//       if (!response.ok) {
//         const errorText = await response.text();
//         throw new Error(`Network response was not ok: ${response.status} - ${errorText}`);
//       }
//       const data = await response.json();
//       setRole(data.roles.map(role => role.authority).join(', '));
//     } catch (error) {
//       console.error("Error fetching role:", error);
//       setError(error.message);
//     }
//   };

//   return (
//     <div>
//       <h1>Check Role</h1>
//       {error && <p>Error: {error}</p>}
//       {role ? (
//         <p>Logged in as: {role}</p>
//       ) : (
//         <Login onLogin={handleLogin} />
//       )}
//     </div>
//   );
// };

// export default Auth;