const defaultHeaders = {
    'Content-Type': 'application/json',
};

const defaultParams = {};

const request = (url, method, headers, params, body) => {
    const requestParams = {
        method,
        headers: {
            ...defaultHeaders,
            ...headers,
        },
        ...defaultParams,
        ...params,
    };

    if (body) {
        requestParams.body = JSON.stringify(body);
    }

    return new Promise((resolve, reject) => {
        fetch(url, {
                ...requestParams,
            })
            .then(response => response.json())
            .then(data => {
                console.log(url, data);
                resolve(data);
            })
            .catch(err => {
                reject(err);
            });
    });
};

export const get = (url, data = {
    headers: {},
    params: {}
}) => request(url, 'GET', data.headers, data.params, null);

export const post = (url, data = {
        headers: {},
        params: {},
        body: {}
    }) =>
    request(url, 'POST', data.headers, data.params, data.body);

export const options = (url, data = {
        headers: {},
        params: {},
        body: {}
    }) =>
    request(url, 'OPTIONS', data.headers, data.params, null);