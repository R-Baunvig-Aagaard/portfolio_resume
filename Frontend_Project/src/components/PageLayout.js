// From SOLO22
import { Outlet } from "react-router-dom";
import { Navbar } from './Navbar';

export function PageLayout() {
  return (
    <>
      
      <Navbar />
      <Outlet />
      <footer>
        <p>Rasmus Baunvig Aagaard - AU532459</p>
      </footer>
    </>
  )
}