import React, { useEffect, useState } from 'react';
import './NewModel.css';

export default function ModelList() {
    const [data, setData] = useState([]);
    
    
        const fetchData = async () => {
        const response = await fetch('https://localhost:7181/api/Models', {
                    method: 'GET', // Or DELETE
                    credentials: 'include',
                    headers: {
                    'Authorization': 'Bearer ' + localStorage.getItem("token"),
                    'Content-Type': 'application/json'
                    }
                    });
            if (response.ok) {
              const data = await (response.json());
              
              setData(data);
            }
        }
                
        

      useEffect(() => {
        fetchData()
      }, [])

    return(
        <div className="modelList-wrapper">
            <h2>List of active models</h2>
            {data.length > 0 && (
            <ul>
                {data.map(model => (
                <ul key={model.efModelId}>{model.firstName} {model.lastName}</ul>
                ))}
            </ul>
            )}
      </div>
  );

}
