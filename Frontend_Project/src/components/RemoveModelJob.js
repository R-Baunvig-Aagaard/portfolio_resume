import React, { useState } from 'react';
import './NewModel.css';

export default function RemoveModelJob() {
    
    const [jobId, setJobId] = useState("");
    const [modelId, setModelId] = useState("");
    
    
    let handleSubmit = async e => {
        e.preventDefault();
        let url = 'https://localhost:7181/api/Jobs/' + jobId + '/model/' + modelId
            fetch(url, {
                method: 'DELETE', // Or PUT
                body: JSON.stringify({
                    jobId: jobId,
                    modelId: modelId
                    
                }), // assumes your data is in a
                // form object on your instance.
                credentials: 'include',
                headers: {
                'Authorization': 'Bearer ' + localStorage.getItem("token"),
                'Content-Type': 'application/json'
                }
        }).then(responseJson => {
        this.response = responseJson;
        })
        //.catch(error => alert('Something bad happened: ' + error));
        }

    return(
    <div className="newModel-wrapper ">
        <h2>Please add Job and model information</h2>
        <form onSubmit={handleSubmit}>
            <label>
                <p>Job ID</p>
                <input type="text"onChange={e => setJobId(e.target.value)} />
            </label>
            <label>
                <p>Model ID</p>
                <input type="text" onChange={e => setModelId(e.target.value)} />
            </label>
            <div>
                <button type="submit">Submit</button>
            </div>
        </form>
    </div>
  );

}
