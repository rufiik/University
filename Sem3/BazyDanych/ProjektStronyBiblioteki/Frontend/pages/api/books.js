export default function handler(req, res) {
    const books = [
      { id: 1, title: "Władca Pierścieni", author: "J.R.R. Tolkien", genre: "Fantasy" },
      { id: 2, title: "Harry Potter", author: "J.K. Rowling", genre: "Fantasy" },
      { id: 3, title: "Hobbit", author: "J.R.R. Tolkien", genre: "Fantasy" },
    ];
  
    if (req.method === 'GET') {
      res.status(200).json(books);
    } else {
      res.setHeader('Allow', ['GET']);
      res.status(405).end(`Method ${req.method} Not Allowed`);
    }
  }
  