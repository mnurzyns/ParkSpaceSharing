import React, {useEffect, useState} from 'react';
import {AuthControllerApi, Configuration, UserControllerApi, UserDto} from "@/client";
import {useSession} from "next-auth/react";
import {mockProviders} from "next-auth/client/__tests__/helpers/mocks";
import credentials = mockProviders.credentials;

const Settings = () => {
  const [dto, setDto] = useState<UserDto>({});
  const {data: session } = useSession();

  useEffect(() => {
    if (session) new UserControllerApi()
        .getOne(session.user.id)
        .then(({data}) => {
          setDto(data);
        })
  }, [session]);

  const handleSubmit = async (e: any) => {
    e.preventDefault();
    await new UserControllerApi({ accessToken: session.user?.token } as Configuration)
        .patchOne(session.user.id, dto)
        .catch(console.log); // TODO(papaj-na-wrotkach): handle errors
  };

  return (
      <div className="container p-6 mx-auto">
        <form onSubmit={handleSubmit} className="card">
          <h2 className="card-title">Settings</h2>
        <div className="form-control">
            <label className="label">
                <span className="label-text">Username</span>
            </label>
            <input
                type="text"
                className="input input-ghost"
                value={dto.username}
                onChange={(e) => setDto(old => ({ ...old, username: e.target.value}))}
                required={false}
            />
        </div>
        <div className="form-control">
          <label className="label">
            <span className="label-text">Email</span>
          </label>
          <input
              type="email"
              className="input input-ghost"
              value={dto.email}
              onChange={(e) => setDto(old => ({ ...old, email: e.target.value}))}
              required={false}
          />
        </div>
        <div className="form-control">
            <label className="label">
                <span className="label-text">Phone number</span>
            </label>
            <input
                type="text"
                className="input input-ghost"
                value={dto.phone}
                onChange={(e) => setDto(old => ({ ...old, phone: e.target.value}))}
                required={false}
            />
        </div>
        <div className="form-control">
            <label className="label">
                <span className="label-text">New password</span>
            </label>
            <input
                type="password"
                className="input input-ghost"
                placeholder="••••••••"
                value={dto.password}
                onChange={(e) => setDto(old => ({ ...old, password: e.target.value}))}
                required={false}
            />
        </div>
        {/*<div className="form-control">*/}
        {/*    <label className="label">*/}
        {/*        <span className="label-text">Confirm new password</span>*/}
        {/*    </label>*/}
        {/*    <input*/}
        {/*        type="password"*/}
        {/*        className="input input-ghost"*/}
        {/*        placeholder="••••••••"*/}
        {/*        //onChange={} //TODO(papaj-na-wrotkach): validate if the password is the same as in previous input*/}
        {/*        required*/}
        {/*    />*/}
        {/*</div>*/}
        <div className="form-control mt-6">
          <button type="submit" className="btn btn-primary">Save Changes</button>
        </div>
      </form>
    </div>
  );
};

export default Settings;
