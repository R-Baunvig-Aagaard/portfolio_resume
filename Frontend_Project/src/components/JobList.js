import React, { useEffect, useState } from 'react';
import './NewModel.css';

export default function JobList() {
    const [data, setData] = useState([]);
    
    
        const fetchData = async () => {
        const response = await fetch('https://localhost:7181/api/Jobs', {
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
            <h2>List of active Jobs</h2>
            {data.length > 0 && (
            <ul>
                {data.map(job => (
                <ul key={job.jobId}>{job.customer} - {job.location}</ul>
                ))}
            </ul>
            )}
      </div>
  );

}
